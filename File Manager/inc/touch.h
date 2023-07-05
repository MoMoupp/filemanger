#ifndef _TOUCH_
#define _TOUCH_


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/input.h>

#define M_ABS_Y 0xaf //上下滑动标志
#define M_ABS_X 0xbf // 左右滑动标志
#define M_TOUH_ 0xcf //触摸标志
/// @brief 触摸，获得x，y坐标 滑动，获得坐标差
/// @param ts_x x坐标
/// @param ts_y y坐标
/// @param code 操控标志
/// @param abs_v 得到坐标差需要一个整型指针接收
/// @return 滑动操作 U 上滑 D下滑 L左滑 R右滑
char touch(int *ts_x, int *ts_y, int code, int *abs_v);


// 键盘输入
//char *key_input();
#endif