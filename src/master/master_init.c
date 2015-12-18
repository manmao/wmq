#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

#include "common_define.h"
#include "server_sockopt.h"
#include "master_init.h"
#include  "global.h"

static void handleterm(int sig)  
{  
	printf("SIGTERM GET\n");
    return;  
}

int master_server_init(int argc,char *argv[])
{
	pid_t pd;
	pd=fork();

	if(pd<0)
	{
		errExit("FILE:%s,LINE:%d",__FILE__,__LINE__);
	}
	//父进程
	if(pd>0){

	}else if(pd==0){ //子进程

		SERVER *master_server;
		init_server(&master_server,CONF.master.port);
		start_listen(master_server);
	    //printf("test end\n");

	}	
}