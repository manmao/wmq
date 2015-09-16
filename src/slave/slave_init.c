#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

#include "common_define.h"
#include "slave_init.h"
#include "server_sockopt.h"

static void handleterm(int sig)  
{  
	printf("SIGTERM GET\n");
    return;  
}

//初始化slave服务器
int slave_server_init(int argnum,char *argv[])
{
	
	pid_t pd;
	pd=fork();
	
	if(pd<0)
	{
		errExit("FILE:%s,LINE:%d",__FILE__,__LINE__);
	}

	//父进程
	if(pd>0)
	{
		/*while(1)  
        {  
            signal(SIGTERM, handleterm);  
            pause();  
        }*/
	}else if(pd==0){ //子进程
		SERVER *slave_server;
		init_server(&slave_server);
		start_listen(slave_server);
	   //printf("test end\n");
	}	
}