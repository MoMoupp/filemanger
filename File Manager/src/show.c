#include "show.h"
#include "getpic.h"
#include <string.h>
/// @brief 没用到
/// @param lcd_buf 
/// @param list_buf 
void show_list(char *lcd_buf, char *list_buf)
{
    if (list_buf == NULL)
    {
        printf("list_buf flase\n");
        return;
    }
    int max = 0;
    // 绘制bmp图片
    char *pb;
    char *p;
    for (int a = 0; a < 20; a++)
    {
        pb = list_buf + (((a % 5) * 160) * 3 + (a / 5 * 120 * 800) * 3);
        p = lcd_buf + (((a % 5) * 160) * 4 + (a / 5 * 120 * 800) * 4);
        for (int i = 0; i < 120; i++)
        {
            for (int j = 0; j < 160; j++)
            {
                memcpy(p + j * 4, pb + j * 3, 3);
            }
            p += 800 * 4;
            pb += 800 * 3;
        }
    }
}

/// @brief 字体有关
/// @param s_font 
/// @param width 
/// @param height 
/// @param size 
/// @return 
bitmap *get_font(char *s_font, int width, int height, int size)
{
    // 创建字库对象
    font *f = fontLoad("./simfang.ttf");
    if (f == NULL)
    {
        printf("fontload error\n");
        return NULL;
    }

    // 设置字体大小
    fontSetSize(f, size);

    // screen-绘制文字区域
    bitmap *screen = createBitmapWithInit(width, height, 4, 0xFFFFFF00); // 创建用显示汉字的像素空间
    // 把汉字绘制到文件区域
    fontPrint(f, screen, 0, 4, s_font, 0x00000000, 0); // RGBA
    // 把文件绘制区域显示在lcd上

    // 销毁汉字显示区域
    // destroyBitmap(screen);
    // 销毁字库
    fontUnload(f);
    return screen;
}

void get_ui_buf(char *uifile, char *uibuf)
{
    // 打开bmpfile文件
    FILE *file = fopen(uifile, "r");
    if (file == NULL)
    {
        perror(uifile);
        return;
    }
    fseek(file, 54, SEEK_SET);
    int h_bit = 0;  // 计算填充字节
    int rd_num = 0; // 记录读取次数
    while (1)
    {
        size_t f = fread(uibuf + (rd_num * 160 * 3), 1, 160 * 3, file);
        fseek(file, h_bit, SEEK_CUR);
        if (f == 0)
        {
            perror("over");
            break;
        }
        rd_num++;
    }
    fclose(file);
}

char *get_coin(char *coinfile, int w, int h)
{
    // 打开bmpfile文件
    FILE *file = fopen(coinfile, "r");
    if (file == NULL)
    {
        perror(coinfile);
        return NULL;
    }
    fseek(file, 54, SEEK_SET);
    char *uibuf = calloc(w * h * 3, 1);
    int h_bit = 0; // 计算填充字节
    int rd_num = 0;                      // 记录读取次数
    while (1)
    {
        size_t f = fread(uibuf + (rd_num * w * 3), 1, w * 3, file);
        fseek(file, h_bit, SEEK_CUR);
        if (f == 0)
        {
            perror("over");
            break;
        }
        rd_num++;
    }
    fclose(file);
    return uibuf;
}

/// @brief 显示目录图形化界面
/// @param mg 链表管理头指针
/// @param list_buf lcd映射
/// @param abs //滑动后导致文件显示后隐藏的数量
void get_show_list_buf(manger *mg, char *list_buf, int abs)
{
    list *t = mg->head;
    int max = 0;
    int n = abs;
    if (n >= mg->filecounts)
    {
        n = mg->filecounts - 7;
    }
    if (n < 0)
    {
        n = 0;
    }
    char *uibuf = calloc(120 * 160 * 3, 1);
    char *coinbuf = get_coin("p_coin.bmp", 80, 60);
    char *p;
    get_ui_buf("dir2.bmp", uibuf);
    while (t)
    {
        if (max >= 20)
        {
            break;
        }
        while (n)
        {
            t = t->next;
            n--;
        } 
        p = list_buf + (((max % 5) * 160) * 4 + (max / 5 * 120 * 800) * 4);

        char *pf = p;
        char *pbmp = p;
        bitmap *font;
        int nlen = strlen(t->name);
        if (nlen > 8)
        {
            if (nlen>13)
            {
                font = get_font(t->name, 102, 28, 10);
            }
            else
            {
                font = get_font(t->name, 102, 28, 15);
            }        
        }
        else
        {
            font = get_font(t->name, 102, 28, 24);
        }
        
        
        // 倒转
        char *t_bmp = uibuf + 160 * (120 - 1) * 3;
        // 绘制bmp图片
        for (int i = 0; i < 120; i++)
        {
            for (int j = 0; j < 160; j++)
            {
                memcpy(p + j * 4, t_bmp + j * 3, 3);
            }
            p += 800 * 4;
            t_bmp -= 160 * 3;
        }
        
        pf += (40 + 81 * 800) * 4;
        char *font_p = font->map;
        for (int i = 0; i < font->height; i++)
        {
            for (int j = 0; j < font->width; j++)
            {
                pf[j * 4] = font_p[j * 4 + 2];
                pf[j * 4 + 1] = font_p[j * 4 + 1];
                pf[j * 4 + 2] = font_p[j * 4];
            }
            pf += 800 * 4;
            font_p += font->width * font->byteperpixel;
        }
        destroyBitmap(font);
        
        if (t->T_type == 8)
        {
            if (strcmp(t->type, ".bmp") == 0)
            {
                char picfile[256];
                sprintf(picfile, "%s/%s", t->pathname, t->name);
                get_bmp(t, picfile);
                printf("%s finsh\n", picfile);
                char *littpic_r = zoom_bmp(t, 80, 60);
                char *littpic = littpic_r;
                pbmp += (38 + 20 * 800) * 4;

                for (int i = 0; i < 60; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        memcpy(pbmp + j * 4, littpic + j * 3, 3);
                    }
                    pbmp += 800 * 4;
                    littpic += 80 * 3;
                }
                free(littpic_r);
            }
            else if (strcmp(t->type, ".jpg") == 0)
            {
                char picfile[256];
                sprintf(picfile, "%s/%s", t->pathname, t->name);
                get_jpeg(t, picfile);
                printf("%s finsh\n", picfile);
                char *littpic_r = zoom_bmp(t, 80, 60);
                char *littpic = littpic_r;
                pbmp += (38 + 20 * 800) * 4;

                for (int i = 0; i < 60; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        memcpy(pbmp + j * 4, littpic + j * 3, 3);
                    }
                    pbmp += 800 * 4;
                    littpic += 80 * 3;
                }
                free(littpic_r);
            }
            
            else
            {
                // 倒转
                char *t_coin = coinbuf + 80 * (60 - 1) * 3;
                pbmp += (38 + 20 * 800) * 4;

                for (int i = 0; i < 60; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        memcpy(pbmp + j * 4, t_coin + j * 3, 3);
                    }
                    pbmp += 800 * 4;
                    t_coin -= 80 * 3;
                }
            }
        }
        t = t->next;
        max++;
    }
    
    free(coinbuf);
    free(uibuf);
}

static void show_f(LcdDevice *lcd, int x, int y, bitmap *mp)
{
    // 根据lcd，bmp尺寸和x，y位置计算要绘制图片的宽dw，高dh
    int dw, dh;
    // 保存lcd绘制开始位置(x,y)指针
    unsigned char *p = lcd->mptr;
    // 保存bmp图片数据的开始位置p指针
    unsigned char *px = NULL;
    // 判断可见区
    if (lcd->lcd_vinfo->yoffset == 0)
    {
        p = lcd->mptr;
    }
    else if (lcd->lcd_vinfo->yoffset == lcd->h)
    {
        p += lcd->w * lcd->h * lcd->pix; // 移到到下一块屏幕，800*480*4
    }
    // 边界处理
    if (x >= 0 && y >= 0)
    {
        // 根据lcd，bmp尺寸和x，y位置计算要绘制图片的宽dw，高dh
        dw = ((x + mp->width) > lcd->w) ? (lcd->w - x) : mp->width;
        dh = ((y + mp->height) > lcd->h) ? (lcd->h - y) : mp->height;

        // 把lcd绘图指针移动x，y位置
        p += (x + y * lcd->w) * 4;
        // 定义一个指针指向图片像素首地址
        px = mp->map;
    }
    // 绘制
    for (int i = 0; i < dh; i++)
    {
        for (int j = 0; j < dw; j++)
        {
            // 从px开始位置拷贝3字节给到p开始位置,之后p和px 往后偏移j个像素
            memcpy(p + j * 4, px + j * mp->byteperpixel, mp->byteperpixel);
        }
        p += lcd->w * 4;                    // lcd绘制指针移动到下一行
        px += mp->width * mp->byteperpixel; // bmp图片指针移动到下一行
    }
}
/// @brief 显示文字
/// @param lcd 屏幕
/// @param s_font 显示的文字
/// @param x 要显示的x坐标
/// @param y y坐标
/// @param width 宽
/// @param height 高
/// @param size 字体大小
void show_font(LcdDevice *lcd, char *s_font, int x, int y, int width, int height, int size)
{
    // 创建字库对象
    font *f = fontLoad("./simfang.ttf");
    if (f == NULL)
    {
        printf("fontload error\n");
        return;
    }

    // 设置字体大小
    fontSetSize(f, size);

    // screen-绘制文字区域 （800x80x4）
    bitmap *screen = createBitmapWithInit(width, height, 4, 0xFFFFFF00); // 创建用显示汉字的像素空间
    // 把汉字绘制到文件区域
    fontPrint(f, screen, 0, 5, s_font, 0x00000000, 0); // RGBA
    // 把文件绘制区域显示在lcd上
    // 显示
    show_f(lcd, x, y, screen);

    // 销毁汉字显示区域
    destroyBitmap(screen);
    // 销毁字库
    fontUnload(f);
}
/// @brief 在目录下浏览图片
/// @param lcd 
/// @param mp 
void show_pic_inlist(LcdDevice *lcd, list *mp)
{

    if (lcd == NULL || mp == NULL)
    {
        return;
    }
    char *t_bmp = zoom_bmp(mp, 800, 480);
    char *f_t_bmp = t_bmp;

    int d_pix = mp->pix;
    // 比较图片和屏幕的色深大小
    if (mp->pix > lcd->pix)
    {
        d_pix = lcd->pix;
    }
    // 倒转
    // char *t_bmp = mp->RGB_data + mp->w * (mp->h - 1) * mp->pix;

    // 临时指针
    unsigned char *p = lcd->mptr;
#if 0
    // 多缓冲显示
    int show_num = 0;
    if (lcd->lcd_vinfo->yoffset == 0)
    {
        p += lcd->w * lcd->h * lcd->pix; // 移到到下一块屏幕，800*480*4
        show_num = 1;
    }
    else if (lcd->lcd_vinfo->yoffset == lcd->h)
    {
        p = lcd->mptr;
        show_num = 0;
    }
    printf("P:%d\n", show_num);
#endif
    // 绘制bmp图片
    for (int i = 0; i < 480; i++)
    {
        for (int j = 0; j < 800; j++)
        {
            memcpy(p + j * lcd->pix, t_bmp + j * mp->pix, d_pix);
        }
        p += lcd->w * lcd->pix;
        t_bmp += 800 * mp->pix;
    }
    free(f_t_bmp);
#if 0
    // 可见区与虚拟区的偏移量
    lcd->lcd_vinfo->yoffset = show_num * lcd->h;
    lcd->lcd_vinfo->xoffset = 0;
    // 写入设备信息
    ioctl(lcd->lcd_fd, FBIOPAN_DISPLAY, lcd->lcd_vinfo);
#endif
}
/// @brief 任意位置显示jpg
/// @param lcd 
/// @param filename 
/// @param x 
/// @param y 
void show_gif_jpeg(LcdDevice *lcd, char *filename, int x, int y)
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
        // fprintf(stderr, "can't open %s\n", filename);
        perror("can't open");
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile); // 把解码文件和定义了解码对象进行关联

    // 3.读取要解码的JPEG图片的文件头
    jpeg_read_header(&cinfo, TRUE);

    // 4.设置解码参数，如果不设置则使用默认值即可 （可选）

    // 5开始解码
    jpeg_start_decompress(&cinfo);

    int dw = cinfo.output_width;  // 要绘制的宽
    int dh = cinfo.output_height; // 要绘制的高
    // 边界处理
    if (x + cinfo.output_width > lcd->w)
    {
        dw = lcd->w - x;
    }
    if (y + cinfo.output_height > lcd->h)
    {
        dh = lcd->h - y;
    }

    // 申请了一块内存，内存的大小就是JPEG图片一行的数据大小 = 宽像素 * 色深
    int row_stride;
    row_stride = cinfo.output_width * cinfo.output_components;
    // 申请一行大小的堆内存，用户来存储读取的一行数据

    unsigned char *buffer = (unsigned char *)malloc(row_stride);
    // 防止地址丢失
    unsigned char *p = lcd->mptr;

    // 把指针p移动到要绘制的开始位置
    p = p + (x * lcd->pix + (y * lcd->w * lcd->pix));

    // 6.循环读取JPEG图片的每一行的数据
    while (cinfo.output_scanline < dh)
    {
        // 每次只扫描一行
        jpeg_read_scanlines(&cinfo, &buffer, 1);

        // 扫描一行数据，并写入到LCD，需要用户自行修改  JPEG图片的像素的存储顺序 RGB
        int i = 0;
        for (i = 0; i < dw; i++)
        {
            p[i * 4] = buffer[i * 3 + 2];     // B
            p[i * 4 + 1] = buffer[i * 3 + 1]; // G
            p[i * 4 + 2] = buffer[i * 3];     // R
        }
        p += (lcd->w) * (lcd->pix); // lcd映射移到到下一行
    }
    // 7.解码完成
    // jpeg_finish_decompress(&cinfo);
    // 8.释放对象
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    free(buffer);
}