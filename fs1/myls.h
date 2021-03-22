#ifndef _MYLS_H_
#define _MYLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <glob.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

#define PATHSIZ 1024

char* change_mode(long int  st_mode ,char c_mode[15] ); //将权限转换为字符串

//char* gettime(time_t time ,char *timeret); //将时间大整数转换成所需字符串

//int is_noloop(char * pathname); //去除. .. 这种情况发生的情况

void myls_t(char * p);

void myls_a(char * p);

void myls_n(char * p);

void myls_l(char * p);

#endif