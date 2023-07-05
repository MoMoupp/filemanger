#ifndef _FINDIR_
#define _FINDIR_
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "doublelist.h"

int Show_aDir(const char *PathName, manger *mg);
#endif