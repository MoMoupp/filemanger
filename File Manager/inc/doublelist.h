#ifndef _DOULEL_
#define _DOULEL_
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
//链表结构体
typedef struct link_list
{
    int T_type;
    int No;  //文件序号
    char *name;
    char *pathname;
    char *type;
    int w;
    int h;
    int pix;
    unsigned long size;
    unsigned int h_bit;
    unsigned int rgb_size;
    char *RGB_data;
    struct link_list *next;
    struct link_list *prev;
} list;
//管理头节点
typedef struct manger
{   
    int filecounts; //文件数
    list *head;
    list *tail;
} manger;
/// @brief 创建链表
/// @param  无
/// @return 一个新的管理头节点的指针
manger *creat_mg(void);

/// @brief 尾插法
/// @param mg 管理头节点
/// @param ls 新的结构体的指针
void TailInsert(manger *mg, list *ls);

/// @brief 清空链表
/// @param mg 管理头节点的指针
void clearlist(manger *mg);

#endif