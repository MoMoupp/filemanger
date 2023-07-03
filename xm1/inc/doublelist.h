#ifndef _DOULEL_
#define _DOULEL_
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct link_list
{
    int T_type;
    int No;
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

typedef struct manger
{   
    int filecounts;
    list *head;
    list *tail;
} manger;

manger *creat_mg(void);
void TailInsert(manger *mg, list *ls);
void clearlist(manger *mg);

#endif