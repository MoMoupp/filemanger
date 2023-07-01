#ifndef _SHOW_
#define _SHOW_

#include "doublelist.h"
#include "font.h"
#include <string.h>
#include <stdio.h>
#include "lcd_dev.h"

void show_list(char *lcd_buf, char *list_buf);
bitmap *get_font(char *s_font, int width, int height, int size);
void get_ui_buf(char *uifile, char *uibuf);
void get_show_list_buf(manger *mg, char *list_buf,int abs);
void show_font(LcdDevice *lcd, char *s_font, int x, int y, int width, int height, int size);
void show_pic_inlist(LcdDevice *lcd, list *mp);
void show_gif_jpeg(LcdDevice *lcd, char *filename, int x, int y);
#endif