#include "finddir.h"
#include "touch.h"
#include "show.h"


int get_NO(int y);

int main(void)
{
    manger *manger1 = creat_mg();
    LcdDevice *lcd= create_lcd("/dev/fb0");
    Show_aDir("/zayu", manger1);
    char *list_buf;
    int ABS_d = 0;
    int x, y = -1;
    while (1)
    {

        list *temp = manger1->head;
        //uibuf = calloc(80 * 800 * 3, 1);        // 36
        list_buf = calloc(80 * 6 * 800 * 3, 1); // 37
        
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
        //get_ui_buf("1.bmp", uibuf);
        
        get_show_list_buf(manger1, list_buf, ABS_d);

        show_list(lcd->mptr, list_buf);
        free(list_buf);
        //free(uibuf);

        int abs_v = 0;
        char st = touch(&x, &y, M_ABS_Y,&abs_v);
        int abs_c = abs_v / 80;
        if (abs_c >= 6)
        {
            abs_c = 5;
        }

        printf("%c\n", st);
        if (st == 'U')
        {
            printf("f:%d\n", manger1->filecounts);
            if (ABS_d + abs_c < manger1->filecounts - 2)
            {
                printf("abs++\n");
                ABS_d+=abs_c;
            }
        }
        else if (st == 'D')
        {
            printf("abs--\n");
            ABS_d-=abs_c;
        }
        
        else if (st == 'C')
        {   
            int counts = 0;
            counts = get_NO(y);
            counts += ABS_d;
            ABS_d = 0;
            
            if (counts<0)
            {
                counts = get_NO(y);
            }
            if (counts > manger1->filecounts)
            {
                counts = get_NO(y) + manger1->filecounts - 2;
            }
            show_font(lcd, "读取中..", 260, 400, 400, 80, 60);
            list *temp1 = manger1->head;
            while (1)
            {

                if (temp1 == NULL)
                {
                    break;
                }
                if (temp1->No == counts)
                {

                    char pname[strlen(temp1->pathname) + strlen(temp1->name)];//-
                    
                    if (strcmp(temp1->name, "..") == 0 && temp1->T_type == 4)
                    {
                        printf("up\n");
                        char *s = strrchr(temp1->pathname, '/');
                        s[0] = '\0';
                        strcpy(pname, temp1->pathname);
                        clearlist(manger1);
                        if (strlen(pname) == 0)
                        {
                            Show_aDir("/", manger1);
                        }
                        else
                        {
                            Show_aDir(pname, manger1);
                        }
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


                    if (temp1->T_type == 8 && strcmp(temp1->type, ".bmp") == 0)
                    {
                        printf("show bmp\n");
                        touch(&x, &y, M_ABS_X, &abs_v);
                        break;
                    }
                    else if (temp1->T_type == 8 && strcmp(temp1->type, ".jpg") == 0)
                    {
                        printf("show jpg\n");
                        touch(&x, &y, M_ABS_X, &abs_v);
                        break;
                    }   
                    else
                    {
                        
                    }
                    
                    break;                 
                }
                temp1 = temp1->next;
            }
        }
    }
}

int get_NO(int y)
{
    return (y / 80 + 1);
}