#ifndef _TOUCH_
#define _TOUCH_


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/input.h>

#define M_ABS_Y 0xaf
#define M_ABS_X 0xbf
#define M_TOUH_ 0xcf
/// @brief 触摸，获得x，y坐标
/// @param ts_x x坐标
/// @param ts_y y坐标
char touch(int *ts_x, int *ts_y, int code, int *abs_v);
// 键盘输入
char *key_input();
#endif