#ifndef __SIG_HANDLER_H_
#define __SIG_HANDLER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <execinfo.h>

/* 定义一个数据结构用来保存信号 */
typedef struct sig_info
{
    int     signum;
    char    signame[20];
} sig_info_t;

/* 我们自定义的信号处理函数 */
extern void blackbox_handler(int sig);

extern void registe_sig_handler();

#endif