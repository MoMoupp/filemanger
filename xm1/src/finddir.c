#include"finddir.h"
#include "getpic.h"

int Show_aDir(const char *PathName, manger *mg)
{

    // 定义一个变量用来记录文件顺序
    int count = 0;
    // 打开目录文件
    DIR *dirp = opendir(PathName);
    if (NULL == dirp)
    {
        // perror("open dir error");
        fprintf(stderr, "open dir [%s] error:%s\n", PathName, strerror(errno));
        mg->filecounts = count;
        return count;
    }

    // printf("已经打开%s目录文件进行搜索...\n" , PathName);
    char Path[128] = {0};

    while (1)
    {
        
        // 读取目录项
        struct dirent *Info = readdir(dirp);
        if (Info == NULL)
        {
            fprintf(stderr, "read dir %s error:%s\n", PathName, strerror(errno));
            break;
        }
        // 新建链表
        list *new = malloc(sizeof(list));
        new->next = new->prev = NULL;
        // printf("Ino:%ld Name:%s Off:%ld Reclen:%d Type:%d\n" ,
        //         Info->d_ino ,Info->d_name , Info->d_off ,Info->d_reclen ,Info->d_type );

        // 判断是否为目录文件
        if (Info->d_type == 4 && strcmp(Info->d_name, "."))
        {
            sprintf(Path, "%s/%s", PathName, Info->d_name);
            new->T_type = 4;
            new->name = malloc(256);
            new->pathname = malloc(256);
            new->type = malloc(256);
            strcpy(new->type, "dir");
            strcpy(new->pathname, PathName);
            strcpy(new->name, Info->d_name);
            count++;
            new->No = count;
            TailInsert(mg, new);
            printf("{Path:%s}\n", Path);
        }
        // 判断如果不是目录
        if (Info->d_type == 8)
        {
            
            //保存普通文件的基本数据
            new->type = malloc(256);
            char *p = strrchr(Info->d_name, '.');
            if (p == NULL)
            {
                
                strcpy(new->type, "NULL");
                
            }
            else
            {
                strcpy(new->type, p);
                
            }
            count++;
            new->No = count;
            new->T_type = 8;
            
            new->name = malloc(256);
            strcpy(new->name, Info->d_name);
            
            new->pathname = malloc(256);
            strcpy(new->pathname, PathName);
            
            char picfile[256];
            struct stat buf;
            sprintf(picfile, "%s/%s", PathName, Info->d_name);
            
            stat(picfile, &buf);
            new->size = buf.st_size;
            
            printf("shit:%s,name:%s\n", new->type,new->name);
            if (strcmp(new->type, ".bmp") == 0)
            {
                new->RGB_data = NULL;
                //get_bmp(new, picfile);
                printf("%s finsh\n", picfile);
            }
            else if (strcmp(new->type, ".jpg") == 0)
            {
                new->RGB_data = NULL;
                //get_jpeg(new, picfile);
                printf("%s finsh\n", picfile);
            } 
           
            // 插入链表
            TailInsert(mg, new);
           
        }
        mg->filecounts = count;
        
    }
    // 关闭目录
    closedir(dirp);

    return count;
}