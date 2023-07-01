#include"show.h"
#include"getpic.h"
#include <string.h>
void show_list(char *lcd_buf,char *list_buf)
{   
    if (list_buf==NULL)
    {
        printf("list_buf flase\n");
        return;
    } 
    // 临时指针
    char *p = lcd_buf;   

    // 绘制bmp图片
    for (int i = 0; i < 480; i++)
    {
        for (int j = 0; j < 800; j++)
        {
            memcpy(p + j * 4, list_buf + j * 3, 3);
        }
        p += 800 * 4;
        list_buf += 800 * 3;
    } 
}

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
    int h_bit = 0; // 计算填充字节
    int rd_num = 0; // 记录读取次数
    while (1)
    {
        size_t f = fread(uibuf + (rd_num * 800 * 3), 1, 800 * 3, file);
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

void get_show_list_buf(manger *mg, char *list_buf,int abs)
{
    list *t = mg->head;
    int max = 0;
    int n = abs;
/*     if (n >= mg->filecounts)
    {
        n = mg->filecounts - 2;
    }*/
    if (n<0)
    { 
        n = 0;
    }

    char *uibuf = calloc(80 * 800 * 3, 1);
    get_ui_buf("1.bmp", uibuf);
    while (t)
    {
        if (max >= 6)
        {
            break;
        }
        while (n)
        {
            t = t->next;
            n--;
        }
        char *p = list_buf + (80 * max * 800 * 3);
        char *pf = p;
        char *pbmp = p;
        bitmap *font = get_font(t->name, 365, 25, 20);
        
        int d_pix = 3;
        // 倒转
         char *t_bmp = uibuf + 800 * (80 - 1) * 3;
        // 绘制bmp图片
        for (int i = 0; i < 80; i++)
        {
            for (int j = 0; j < 800; j++)
            {
                memcpy(p + j * 3, t_bmp + j * 3, 3);
            }
            p += 800 * 3;
            t_bmp -= 800 * 3;
        }
        
        
        pf += (110 + 30 * 800) * 3;
        char *font_p = font->map;
        for (int i = 0; i < font->height; i++)
        {
            for (int j = 0; j < font->width; j++)
            {
                pf[j * 3] = font_p[j * 4 + 2];
                pf[j * 3 + 1] = font_p[j * 4 + 1];
                pf[j * 3 + 2] = font_p[j * 4];
            }
            pf += 800 * 3;
            font_p += font->width * font->byteperpixel;
        }
        destroyBitmap(font);
        if (strcmp(t->type, ".bmp") == 0 || strcmp(t->type, ".jpg") == 0)
        {
            //printf("litte start\n");
            char *littpic_r = zoom_bmp(t, 96, 60);
            //printf("zoom over\n");
            char *littpic = littpic_r;
            pbmp += (12 + 10 * 800) * 3;
            
            for (int i = 0; i < 60; i++)
            {
                for (int j = 0; j < 96; j++)
                {
                    memcpy(pbmp + j * 3, littpic + j * 3, 3);
                }
                pbmp += 800 * 3;
                littpic += 96 * 3;
            }
            free(littpic_r);
        }
        
        t = t->next;
        max++;
    }
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

