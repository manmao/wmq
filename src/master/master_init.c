#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "common_define.h"
#include "global.h"

#include "server_sockopt.h"

#include "master_init.h"


static
void on_master_handle(struct sock_server *server,struct epoll_event event)
{
	int event_fd=event.data.fd;

	struct sock_pkt recv_pkt;//网络数据包数据结构
	while(1)
	{

		int buflen=recv(event_fd,(void *)&recv_pkt,sizeof(struct sock_pkt),0);

		if(buflen < 0)
		{
			if(errno== EAGAIN || errno == EINTR){ //即当buflen<0且errno=EAGAIN时，表示没有数据了。(读/写都是这样)
              	printf("no data get!!!!\n");
              	return ;
            }else{
              	printf("epoll error %s %d\n",__FILE__,__LINE__);
              	return;                				 //error
            }
		}
		if(buflen==0) 				//客户端断开连接
		{
			server->connect_num--;  //客户端连接数量减1

			/**将文件描述符从epoll队列中移除**/
			deletefd(server->efd,event_fd);

			/*******删除连接队列中的点*******/
			struct conn_node node;
			node.accept_fd=event_fd;
			conn_delete(&server->conn_root,&node);

			//调试信息
			printf("有客户端断开连接了,现在连接数:%d\n",server->connect_num);
			return ;
		}
		else if(buflen>0) //客户端发送数据过来了
		{
			//将数据包加入任务队列
			//threadpool_add_job(server->tpool,handle_pkg,(void *)&recv_pkt);
			//printf("包个数: ==> %d\n",count++);
			//往线程池添加执行单元
		}
	}
}



int master_server_init(int argc,char *argv[])
{
	SERVER *master_server;

	//
	struct server_handler *handler=(struct server_handler *)malloc(sizeof(struct server_handler));
	handler->handle_readable=&on_master_handle;

	init_server(&master_server,CONF.master.port,handler);
	start_listen(master_server); //启动服务器
}
