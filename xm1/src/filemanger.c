#include "filemanger.h"
#include"getpic.h"
inline int get_NO(int x, int y);

int filemanger(LcdDevice *lcd)
{
    manger *manger1 = creat_mg();
    // LcdDevice *lcd= create_lcd("/dev/fb0");
    Show_aDir("/zayu", manger1);
    char *list_buf;
    int ABS_d = 0;
    int x, y = -1;
    while (1)
    {

        list *temp = manger1->head;
        // uibuf = calloc(80 * 800 * 3, 1);        // 36
        //list_buf = calloc(800 * 480 * 3, 1); // 37
        for (int i = 0; i < 800 * 480 ; i++)
        {
            lcd->mptr[i*4] = 0xff;
            lcd->mptr[i * 4 + 1] = 0xff;
            lcd->mptr[i * 4 + 2] = 0xff;
        }
        
        while (1)
        {
            if (temp == NULL)
            {
                break;
            }
            printf("%s\n", temp->name);
            temp = temp->next;
        }
        printf("----------------\n");
        // get_ui_buf("1.bmp", uibuf);

        get_show_list_buf(manger1, lcd->mptr, ABS_d);
        printf("list get\n");
        //show_list(lcd->mptr, list_buf);
       // printf("show list over\n");
        //free(list_buf);
        // free(uibuf);

        int abs_v = 0;
        char st = touch(&x, &y, M_ABS_Y, &abs_v);
        int abs_c = (abs_v / 120 + 1) * 5;
        if (abs_c >= 20)
        {
            abs_c = 3 * 5;
        }

        printf("%c\n", st);
        
        if (st == 'U')
        {
            
            printf("abs++\n");
            ABS_d += abs_c;
            if (ABS_d >= manger1->filecounts)
            {
                ABS_d = manger1->filecounts - 7;
            }
        }
        else if (st == 'D')
        {
            printf("abs--\n");
            ABS_d -= abs_c;
            if (ABS_d<0)
            {
                ABS_d = 0;
            }
            
        }

        else if (st == 'C')
        {
            int counts = 0;
            counts = get_NO(x,y);
            if (counts > manger1->filecounts)
            {
                continue;
            }
            
            printf("NO1:%d ", counts);
            counts += ABS_d;
            ABS_d = 0;
            printf("NO2:%d ", counts);
            if (counts < 0)
            {
                counts = get_NO(x,y);
            }
            if (counts > manger1->filecounts)
            {
                counts = get_NO(x,y) + manger1->filecounts - 7;
            }
            printf("NO:%d\n", counts);
            show_font(lcd, "读取中...", 260, 400, 400, 80, 60);
            list *temp1 = manger1->head;
            while (1)
            {

                if (temp1 == NULL)
                {
                    break;
                }
                if (temp1->No == counts)
                {
                    if (strcmp(temp1->name, "..") == 0 && strcmp(temp1->pathname, "/") == 0)
                    {
                        clearlist(manger1);
                        free(manger1);
                        return 0;
                    }

                    char pname[strlen(temp1->pathname) + strlen(temp1->name)]; //-

                    if (strcmp(temp1->name, "..") == 0 && temp1->T_type == 4)
                    {
                        printf("up\n");
                        char *s = strrchr(temp1->pathname, '/');
                        printf("p1\n");
                        s[0] = '\0';
                        strcpy(pname, temp1->pathname);
                        printf("p2\n");
                        clearlist(manger1);
                        printf("p3\n");
                        if (strlen(pname) == 0)
                        {
                            printf("p4\n");
                            Show_aDir("/", manger1);
                            printf("p5\n");
                        }
                        else
                        {
                            printf("p6\n");
                            Show_aDir(pname, manger1);
                            printf("p7\n");
                        }
                        printf("p8\n");
                        break;
                    }
                    if (temp1->T_type == 4)
                    {
                        sprintf(pname, "%s/%s", temp1->pathname, temp1->name);
                        clearlist(manger1);
                        Show_aDir(pname, manger1);
                        strcpy(pname, "\0");
                        break;
                    }

                    if (temp1->T_type == 8 && (strcmp(temp1->type, ".jpg") == 0 || strcmp(temp1->type, ".bmp") == 0))
                    {
                        if (temp1->RGB_data==NULL)
                        {
                            if (strcmp(temp1->type, ".jpg") == 0)
                            {
                                char picfile[256];
                                sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                get_jpeg(temp1, picfile);
                                printf("%s finsh\n", picfile);
                            }
                            else if (strcmp(temp1->type, ".bmp") == 0)
                            {
                                char picfile[256];
                                sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                get_bmp(temp1, picfile);
                                printf("%s finsh\n", picfile);
                            }
                            
                          }
                        
                        show_pic_inlist(lcd, temp1);
                        while (1)
                        {
                            char c = touch(&x, &y, M_ABS_X, &abs_v);
                            printf("mei\n");
                            //printf("%c\n", c);
                            if (c == 'C')
                            {
                                break;
                            }
                            else if (c == 'L')
                            {
                                while (1)
                                {
                                    if (temp1->next == NULL)
                                    {
                                        while (temp1->prev)
                                        {
                                            temp1 = temp1->prev;
                                        }
                                        if (temp1->T_type == 8 && (strcmp(temp1->type, ".jpg") == 0 || strcmp(temp1->type, ".bmp") == 0))
                                        {
                                            if (temp1->RGB_data == NULL)
                                            {
                                                if (strcmp(temp1->type, ".jpg") == 0)
                                                {
                                                    char picfile[256];
                                                    sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                    get_jpeg(temp1, picfile);
                                                    printf("%s finsh\n", picfile);
                                                }
                                                else if (strcmp(temp1->type, ".bmp") == 0)
                                                {
                                                    char picfile[256];
                                                    sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                    get_bmp(temp1, picfile);
                                                    printf("%s finsh\n", picfile);
                                                }
                                            }
                                            show_pic_inlist(lcd, temp1);
                                            break;
                                        }
                                    }
                                    
                                    temp1 = temp1->next;

                                    if (temp1->T_type == 8 && (strcmp(temp1->type, ".jpg") == 0 || strcmp(temp1->type, ".bmp") == 0))
                                    {
                                        if (temp1->RGB_data == NULL)
                                        {
                                            if (strcmp(temp1->type, ".jpg") == 0)
                                            {
                                                char picfile[256];
                                                sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                get_jpeg(temp1, picfile);
                                                printf("%s finsh\n", picfile);
                                            }
                                            else if (strcmp(temp1->type, ".bmp") == 0)
                                            {
                                                char picfile[256];
                                                sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                get_bmp(temp1, picfile);
                                                printf("%s finsh\n", picfile);
                                            }
                                        }
                                        show_pic_inlist(lcd, temp1);
                                        break;
                                    }
                                }
                            }
                            else if (c == 'R')
                            {
                                while (1)
                                {
                                    if (temp1->prev == NULL)
                                    {
                                        while (temp1->next)
                                        {
                                            temp1 = temp1->next;
                                        }
                                        if (temp1->T_type == 8 && (strcmp(temp1->type, ".jpg") == 0 || strcmp(temp1->type, ".bmp") == 0))
                                        {
                                            if (temp1->RGB_data == NULL)
                                            {
                                                if (strcmp(temp1->type, ".jpg") == 0)
                                                {
                                                    char picfile[256];
                                                    sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                    get_jpeg(temp1, picfile);
                                                    printf("%s finsh\n", picfile);
                                                }
                                                else if (strcmp(temp1->type, ".bmp") == 0)
                                                {
                                                    char picfile[256];
                                                    sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                    get_bmp(temp1, picfile);
                                                    printf("%s finsh\n", picfile);
                                                }
                                            }
                                            show_pic_inlist(lcd, temp1);
                                            break;
                                        }
                                    }

                                    temp1 = temp1->prev;

                                    if (temp1->T_type == 8 && (strcmp(temp1->type, ".jpg") == 0 || strcmp(temp1->type, ".bmp") == 0))
                                    {
                                        if (temp1->RGB_data == NULL)
                                        {
                                            if (strcmp(temp1->type, ".jpg") == 0)
                                            {
                                                char picfile[256];
                                                sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                get_jpeg(temp1, picfile);
                                                printf("%s finsh\n", picfile);
                                            }
                                            else if (strcmp(temp1->type, ".bmp") == 0)
                                            {
                                                char picfile[256];
                                                sprintf(picfile, "%s/%s", temp1->pathname, temp1->name);
                                                get_bmp(temp1, picfile);
                                                printf("%s finsh\n", picfile);
                                            }
                                        }
                                        show_pic_inlist(lcd, temp1);
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    else
                            {
                                show_font(lcd, "不支持的文件格式", 260, 400, 600, 80, 60);
                                touch(&x, &y, M_ABS_X, &abs_v);
                            }
                }
                temp1 = temp1->next;
            }
        }
    }
}
int get_NO(int x, int y)
{

    return ( ((y / 120) * 5 + (x / 160)) + 1);
}