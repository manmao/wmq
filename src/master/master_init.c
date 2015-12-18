#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

#include "common_define.h"
#include "server_sockopt.h"
#include "master_init.h"
#include "global.h"

static void handleterm(int sig)  
{  
	printf("SIGTERM GET\n");
    return;  
}

int master_server_init(int argc,char *argv[])
{
	SERVER *master_server;
	init_server(&master_server,CONF.master.port,MASTER);
	start_listen(master_server);
}