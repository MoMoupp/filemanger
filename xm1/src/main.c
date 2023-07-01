#include"filemanger.h"
#include"lcd_dev.h"
#include"touch.h"
void open_animation(LcdDevice *lcd);

int main(void)
{
    LcdDevice *lcd = create_lcd("/dev/fb0");
    open_animation(lcd);
    
    while (1)
    {
        show_gif_jpeg(lcd, "main.jpg", 0, 0);
        int x, y, abs;
        touch(&x, &y, M_TOUH_, &abs);
        if (x > 45 && y > 95 && x < 205 && y < 190)
        {
            filemanger(lcd);
        }
        else
        {
            break;
        }
        
    }
    destroy_lcd(lcd);
    return 0;
}

void open_animation(LcdDevice *lcd)
{
    char ck[50];
    char *ck_name[60];

    for (int i = 0; i < 60; i++)
    {
        sprintf(ck, "/zayu/zayubo/oa2/%d.jpg", i);
        ck_name[i] = (char *)malloc(sizeof(ck));
        strcpy(ck_name[i], ck);
    }
    printf("create over\n");

    for (int i = 0; i < 60; i++)
    {
        show_gif_jpeg(lcd, ck_name[i], 0, 0);
        printf(ck_name[i]);
        printf("\n");
    }
    for (int i = 0; i < 60; i++)
    {
        free(ck_name[i]);
    }
}