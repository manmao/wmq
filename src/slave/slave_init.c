#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>

#include "global.h"
#include "common_define.h"
#include "error_hdr.h"

#include "server_sockopt.h"

#include "slave_init.h"


static
int on_slave_handle(struct request *req_pkg_p)
{

}

//初始化slave服务器
int slave_server_init(int argnum,char *argv[])
{
	int idx;
	sscanf(argv[2],"%d",&idx);
	//printf("slave id:%d,total slave server:%d\n",idx,CONF.slave_server_num);
	if(idx > CONF.slave_server_num)
	{
		errExit("slave 服务器编号不存在\n");
	}

	printf("slave listern port:%d\n",CONF.slave[idx-1].port);

	struct server_handler *handler=(struct server_handler *)malloc(sizeof(struct server_handler));
	handler->handle_readable=&on_slave_handle;
    handler->handle_accept=NULL;
    handler->handle_unknown=NULL;
    handler->handle_writeable=NULL;
    handler->handle_urg=NULL;
    handler->handle_sig=NULL;

	struct sock_server *slave_server=NULL;
	init_server(&slave_server,CONF.slave[idx-1].port,handler,0,0);
	start_listen(slave_server);//启动服务器

	return 0;
}

