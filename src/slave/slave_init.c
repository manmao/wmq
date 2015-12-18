#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

#include "common_define.h"
#include "slave_init.h"
#include "server_sockopt.h"
#include "global.h"

static void handleterm(int sig)  
{  
	printf("SIGTERM GET\n");
    return;  
}

//初始化slave服务器
int slave_server_init(int argnum,char *argv[])
{
	int idx;
	sscanf(argv[2],"%d",&idx);
	printf("slave id:%d\n",idx);
	
	SERVER *slave_server;
	init_server(&slave_server,CONF.slave[idx-1].port,SLAVE);
	start_listen(slave_server);
	
}