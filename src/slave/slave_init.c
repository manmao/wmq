#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <errno.h>

#include "slave_init.h"
#include "global.h"
#include "common_define.h"
#include "error_hdr.h"


static void deletefd(int epollfd,int fd){
	struct epoll_event event;
	event.data.fd=fd;
	event.events=EPOLLIN | EPOLLET |  EPOLLPRI;  //ET 模式
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&event);  //删除fd和事件
	close(fd);
}

void on_slave_handle(struct sock_server *server,struct epoll_event events)
{
	int event_fd=events.data.fd;
	struct sock_pkt recv_pkt;//网络数据包数据结构
	while(1)
	{
		int buflen=recv(event_fd,(void *)&recv_pkt,sizeof(struct sock_pkt),0);
		if(buflen < 0)
		{
			if(errno== EAGAIN || errno == EINTR){ //即当buflen<0且errno=EAGAIN时，表示没有数据了。(读/写都是这样)
              	printf("----------no data----------------\n");
              	return ;
            }else{
              	printf("epoll error %s %s\n",__FILE__,__LINE__);
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

//初始化slave服务器
int slave_server_init(int argnum,char *argv[])
{
	int idx;
	sscanf(argv[2],"%d",&idx);
	printf("slave id:%d,total slave server:%d\n",idx,CONF.slave_server_num);
	if(idx > CONF.slave_server_num)
	{
		errExit("slave 服务器编号不存在\n");
	}
	printf("slave :%d\n",CONF.slave[idx-1].port);

	struct server_handler *handler=(struct server_handler *)malloc(sizeof(struct server_handler));
	handler->handle_readable=&on_slave_handle;

	struct sock_server *slave_server;
	init_server(&slave_server,CONF. slave[idx-1].port, handler);
	start_listen(slave_server);//启动服务器
}

