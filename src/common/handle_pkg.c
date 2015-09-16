#include <stdio.h>
#include <string.h>

#include "server_sockopt.h"
#include "common_define.h"


//处理数据包
//数据包处理运行单元
void *handle_pkg(void *arg)
{
	struct sock_pkt *pkg=(struct sock_pkt*)arg;
	printf("type==>%d\n",pkg->type);
	printf("handle--客户端消息:%s\n",pkg->buff);
}