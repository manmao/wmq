#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <execinfo.h>

#include "config.h"
#include "sig_handler.h"
#include "log.h"

/* 增加我们想要捕捉的异常信号，这里列举了6个 */
sig_info_t sigCatch[] = {
    {1, "SIGHUP"}, {2, "SIGINT"}, {3, "SIGQUIT"},

    {6, "SIGABRT"}, {8, "SIGFPE"}, {11, "SIGSEGV"}
};

void blackbox_handler(int sig)
{

    log_write(CONF.lf,LOG_INFO,"Enter blackbox_handler: ");
    log_write(CONF.lf,LOG_INFO,"SIG name is %s, SIG num is %d\n", strsignal(sig), sig);
    int j, nptrs;
#define SIZE 100
    void *buffer[100];
    char **strings;
    
    nptrs = backtrace(buffer, SIZE);

    log_write(CONF.lf,LOG_INFO,"backtrace() returned %d addresses\n", nptrs);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL)
    {
        log_write(CONF.lf,LOG_INFO,"backtrace_symbol");
        exit(EXIT_FAILURE);
    }

    for(j = 0; j < nptrs; j++)
    	log_write(CONF.lf,LOG_INFO,"%s\n", strings[j]);

    log_write(CONF.lf,LOG_INFO,"server exit unormal");
    
    free(strings);
    _exit(EXIT_SUCCESS);
}

void registe_sig_handler(){
	int i=0;

	struct  sigaction   sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = blackbox_handler;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    //register sig
    for (i = 0; i < sizeof(sigCatch)/sizeof(sig_info_t); i++)
    {
        // 注册信号处理函数
        if(sigaction(sigCatch[i].signum, &sa, NULL) < 0)
        {
            return EXIT_FAILURE;
        }
    }

}