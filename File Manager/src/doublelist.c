#include "doublelist.h"

manger *creat_mg(void)
{
    manger *mg = (manger *)malloc(sizeof(manger));
    mg->head = mg->tail = NULL;
    mg->filecounts = 0;
    return mg;
}

void TailInsert(manger *mg, list *ls)
{
    if (mg->head == NULL || mg->tail == NULL)
    {
        mg->head = ls;
        mg->tail = ls;
        return;
    }
    ls->prev = mg->tail;
    // ls->next = mg->head;
    mg->tail->next = ls;
    // mg->head->prev = ls;
    mg->tail = ls;
    return;
}

void clearlist(manger *mg)
{
    list *t = mg->head;
    while (t)
    {
        list *t1 = t;
        t = t->next;
        free(t1->name);
        free(t1->pathname);
        if (strcmp(t1->type, ".bmp") == 0 || strcmp(t1->type, ".jpg") == 0)
        {
            if (t1->RGB_data!=NULL)
            {
                printf("rgb free\n");
                free(t1->RGB_data);
            }
        }
        free(t1->type);
        free(t1);
    }

    mg->head = mg->tail = NULL;
}