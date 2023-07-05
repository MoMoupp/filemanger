#include "filemanger.h"
#include"getpic.h"
/// @brief 坐标转换对应的文件序号
/// @param x x坐标
/// @param y y坐标
/// @return 文件序号
inline int get_NO(int x, int y);

int filemanger(LcdDevice *lcd)
{   
    //创建链表
    manger *manger1 = creat_mg();
    
    Show_aDir("/zayu", manger1);
    int ABS_d = 0;
    int x, y = -1;
    
    while (1)
    {
        //临时指针
        list *temp = manger1->head;
        //清屏白色
        for (int i = 0; i < 800 * 480 ; i++)
        {
            lcd->mptr[i*4] = 0xff;
            lcd->mptr[i * 4 + 1] = 0xff;
            lcd->mptr[i * 4 + 2] = 0xff;
        }
        //debug
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


        //显示图形界面
        get_show_list_buf(manger1, lcd->mptr, ABS_d);
        printf("list get\n");

        //滑动幅度
        int abs_v = 0;
        //触摸或滑动
        char st = touch(&x, &y, M_ABS_Y, &abs_v);
        
        //计算滑走和出现的文件数
        int abs_c = (abs_v / 120 + 1) * 5;
        if (abs_c >= 20)
        {
            abs_c = 3 * 5;
        }
        

        if (st == 'U') //上滑
        {
            
            printf("abs++\n");
            ABS_d += abs_c;
            if (ABS_d >= manger1->filecounts)
            {
                ABS_d = manger1->filecounts - 7;
            }
        }
        else if (st == 'D') //下滑
        {
            printf("abs--\n");
            ABS_d -= abs_c;
            if (ABS_d<0)
            {
                ABS_d = 0;
            }
            
        }

        else if (st == 'C') //点击
        {
            //以下为计算点击的文件序号
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
            
            //开始遍历
            list *temp1 = manger1->head;
            while (1)
            {

                if (temp1 == NULL)
                {
                    break;
                }

                //找到该序号的文件
                if (temp1->No == counts)
                {
                    if (strcmp(temp1->name, "..") == 0 && strcmp(temp1->pathname, "/") == 0) //在根目录下点击..则退出
                    {   
                        //释放内存
                        clearlist(manger1); 
                        free(manger1);
                        return 0;
                    }


                    char pname[strlen(temp1->pathname) + strlen(temp1->name)]; //

                    //点击到..目录则打开上一级目录
                    if (strcmp(temp1->name, "..") == 0 && temp1->T_type == 4)
                    {
                        //以绝对路径打开
                        char *s = strrchr(temp1->pathname, '/');
                        
                        s[0] = '\0';
                        strcpy(pname, temp1->pathname);
                        
                        //打开新的目录前清空一下链表
                        clearlist(manger1);
                        
                        if (strlen(pname) == 0)//上级目录为根目录的情况
                        {  
                            Show_aDir("/", manger1);
                        }
                        else
                        {
                            Show_aDir(pname, manger1);
                        }
                        //结束检索
                        break;
                    }
                    //点击到目录
                    if (temp1->T_type == 4)
                    {
                        sprintf(pname, "%s/%s", temp1->pathname, temp1->name);
                        clearlist(manger1);
                        Show_aDir(pname, manger1);
                        strcpy(pname, "\0");
                        break;
                    }
                    //点击到jpg，bmp文件
                    if (temp1->T_type == 8 && (strcmp(temp1->type, ".jpg") == 0 || strcmp(temp1->type, ".bmp") == 0))
                    {
                        if (temp1->RGB_data==NULL)//rgb数据为空则读取rgb数据
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
                        //放大浏览
                        show_pic_inlist(lcd, temp1);
                        while (1)
                        {
                            char c = touch(&x, &y, M_ABS_X, &abs_v);
                            //点击
                            if (c == 'C')
                            {
                                break;
                            }
                            //左滑
                            else if (c == 'L')
                            {
                                //有点复制，反正是与循环浏览有关
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
                            //右滑
                            else if (c == 'R')
                            { 
                                // 有点复制，反正是与循环浏览有关
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
                        
                        //结束遍历
                        break;
                    }
                    //点击其他文件
                    else
                            {
                                show_font(lcd, "不支持的文件格式", 260, 400, 600, 80, 60);
                                touch(&x, &y, M_ABS_X, &abs_v);
                            }
                }
                //遍历
                temp1 = temp1->next;
            }
        }
    }
}
int get_NO(int x, int y)
{

    return ( ((y / 120) * 5 + (x / 160)) + 1);
}