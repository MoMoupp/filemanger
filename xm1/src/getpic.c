#include"getpic.h"

char *zoom_bmp(list *ls, int new_w, int new_h)
{
    if (ls == NULL)
    {
        return NULL;
    }
    // 申请一个保存目标图片的空间
    char *destmap = malloc(new_w * new_h * 3); // show.c:149

    // 从原图中拷贝数据到目标图中
    for (int i = 0; i < new_h; i++)
    {
        // 拷贝一行
        for (int j = 0; j < new_w; j++)
        {
            // 拷贝一个像素
            destmap[j * 3 + 0 + i * new_w * 3] = ls->RGB_data[(j * ls->w / new_w) * 3 + 0 + (i * ls->h / new_h) * ls->w * 3];
            destmap[j * 3 + 1 + i * new_w * 3] = ls->RGB_data[(j * ls->w / new_w) * 3 + 1 + (i * ls->h / new_h) * ls->w * 3];
            destmap[j * 3 + 2 + i * new_w * 3] = ls->RGB_data[(j * ls->w / new_w) * 3 + 2 + (i * ls->h / new_h) * ls->w * 3];
        }
    }
    //free(mp->data);
    return destmap;
}

void get_bmp(list *new, char *bmpfile)
{
    // 打开bmpfile文件
    FILE *file = fopen(bmpfile, "r");
    if (file == NULL)
    {
        perror(bmpfile);
        return;
    }
    // 读取bmp头

    BitHeader bmphead;
    size_t size = fread(&bmphead, 1, 54, file);
    if (size < 0)
    {
        perror(bmpfile);
        fclose(file);
        return;
    }
    // 计算像素占用的内存空间大小
    new->w = bmphead.biWidth;
    new->h = bmphead.biHeight;
    new->pix = bmphead.biBitCount / 8;
    // new->size = new->w * new->h * new->pix;

    // 申请保存像素数据的空间
    new->RGB_data = (unsigned char *)malloc(new->size);

    // 计算填充的字节数
    new->h_bit = (4 - ((new->w *new->pix) % 4)) % 4;

    char *tp = new->RGB_data + (new->h - 1) * new->w * new->pix;
    int rd_num = 0; // 记录读取次数
    while (1)
    {
        // 一次读取一行
        size_t f = fread(tp - (rd_num * new->w *new->pix), 1, new->w *new->pix, file);
        if (f == 0)
        {
            perror("bmp_over");
            break;
        }
        // 跳过填充的字节
        fseek(file, new->h_bit, SEEK_CUR);
        rd_num++;
    }
    // 返回
    fclose(file);
}

void get_jpeg(list *new, char *filename)
{

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // 1.定义解码对象以及定义错误处理对象，并且进行初始化
    cinfo.err = jpeg_std_error(&jerr); // 把错误处理对象以及解码对象进行关联
    jpeg_create_decompress(&cinfo);

    // 2.指定需要解码的文件，必须以二进制方式打开
    FILE *infile;

    if ((infile = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile); // 把解码文件和定义了解码对象进行关联

    // 3.读取要解码的JPEG图片的文件头
    jpeg_read_header(&cinfo, TRUE);

    // 4.设置解码参数，如果不设置则使用默认值即可 （可选）

    // 5开始解码
    jpeg_start_decompress(&cinfo);
    //
    new->h = cinfo.output_height;
    new->w = cinfo.output_width;
    new->pix = cinfo.output_components;
    // 申请了一块内存，内存的大小就是JPEG图片一行的数据大小 = 宽像素 * 色深
    int row_stride=0;
    row_stride = cinfo.output_width * cinfo.output_components;
    // 申请一行大小的堆内存，用户来存储读取的一行数据
    unsigned char *buffer = (unsigned char *)malloc(row_stride);
    new->RGB_data = (char *)malloc(row_stride * cinfo.output_height);
    char *p = new->RGB_data;
    // 6.循环读取JPEG图片的每一行的数据
    while (cinfo.output_scanline < cinfo.output_height)
    {
        // 每次只扫描一行
        jpeg_read_scanlines(&cinfo, &buffer, 1);

        // 扫描一行数据，并写入到LCD，需要用户自行修改  JPEG图片的像素的存储顺序 RGB
        int i = 0;
        for (i = 0; i < cinfo.output_width; i++)
        {
            p[i * cinfo.output_components] = buffer[i * cinfo.output_components + 2]; // B
            p[i * cinfo.output_components + 1] = buffer[i * cinfo.output_components + 1]; // G
            p[i * cinfo.output_components + 2] = buffer[i * cinfo.output_components];     // R
        }
        p += row_stride; // lcd映射移到到下一行
    }
    //7.解码完成
    jpeg_finish_decompress(&cinfo);
    // 8.释放对象
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    free(buffer);
}