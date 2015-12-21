#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <error.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>

#include "server_sockopt.h"
#include "util.h"

#include "master_init.h"
#include "slave_init.h"

int count=0;

/********************************
*设置套接字属性
*	主要作用是： 
*				开启keepalive属性
*				设置端口和地址可重用*
*				禁用Nagle算法
*  函数名:client_set_sock
*  函数参数：
*  		@param  client 客户端操作实体
*		@param  sfd    和服务器端连接的套接字文件描述符
* 函数返回:
*		@return  null
*********************************/
void  server_set_sock(int sfd){
	int optval; 	   //整形的选项值
	socklen_t optlen;  //整形的选项值的长度
	int err;		   //设置返回结果

    /***设置端口和地址可重用**/
	optval=1;
	optlen=sizeof(optval);
	err=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(char *)&optval,optlen);

	/*******禁用Nagle算法***********/
	/*optval=1;
	optlen=sizeof(optval);
	err=setsockopt(sfd,IPPROTO_TCP,TCP_NODELAY,(char *)&optval,optlen);*/
	/*******设置文件描述符非阻塞*********/
	setnonblock(sfd);
}


/*******************************************
	当客户端可服务器端建立连接时

	添加fd,到epoll内核事件表中

	efd为内核事件表的文件描述符
	fd为添加的fd 文件描述符

********************************************/
static void addfd(int epollfd,int fd){
	struct epoll_event event;
	event.data.fd=fd;
	event.events=EPOLLIN | EPOLLET | EPOLLPRI;   //ET 模式,为fd注册事件
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event); //添加fd和事件
	setnonblock(fd);
}

/***********************
	客户端和服务器端端开连接

	efd为内核事件表的文件描述符
	fd为添加的fd 文件描述符
	
************************/
static void deletefd(int epollfd,int fd){
	struct epoll_event event;
	event.data.fd=fd;
	event.events=EPOLLIN | EPOLLET |  EPOLLPRI;  //ET 模式
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&event);  //删除fd和事件
	close(fd);
}

/*******************************

	修改fd 的epollfd属性

*********************************/
void modfd(int epollfd,int fd,int ev)
{
	struct epoll_event event;
	event.data.fd=fd;
	event.events=ev|EPOLLET|EPOLLONESHOT|EPOLLRDHUP;
	epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}


/**********************************

	函数功能：处理epoll数据连接
	函数参数:
			@param  server ------- SERVER参数

	函数返回：
			@return -------  void

**********************************/
static void handle_accept_event(SERVER *server)
{
	struct sockaddr clientaddr;
	socklen_t addrlen=sizeof(struct sockaddr);  //地址长度
	int a_fd=accept(server->listenfd,(struct sockaddr *)&clientaddr,&(addrlen));
	
	//如果连接成功
	if(a_fd != -1){

		struct conn_type *type=(struct conn_type *)malloc(sizeof(struct conn_type));
		type->node=(struct conn_node *)malloc(sizeof(struct conn_node));
		type->node->accept_fd=a_fd;
		type->node->clientaddr=clientaddr;
		//type->node->do_task ;
		conn_insert(&server->conn_root,type);

		server->connect_num++;
		printf("连接数量 --%d\n",server->connect_num); 				  //用户连接数量
		addfd(server->efd,type->node->accept_fd);

	}
}

/**********************************

	函数功能：处理epoll可读事件
	函数参数:
			@param  server ------- SERVER参数
			@param  events	-------- epoll事件
	函数返回：
			@return -------  void

	这个函数主要用处理epoll可读事件
**********************************/
static void handle_readable_event(SERVER *server,struct epoll_event events)
{
	/*
		dispatch
	*/
	switch(server->type){

		case MASTER:
			on_master_handle(server,events);
			break;

		case SLAVE:
			on_slave_handle(server,events);
			break;
		default:
			;
	}
   
	
}



/**********************************

	函数功能：处理epoll可写事件
	函数参数:
			@param  
			@param  
	函数返回：
			@return -------  void

	这个函数主要用处理epoll可读事件

**********************************/
static void handle_writeable_event()
{

}



/**********************************

	信号注册回调函数:处理SIGKILL

	函数名称：handle_close
	函数参数：
		@param  iSignNo  -----
	函数返回：
		@return  -----  void

**********************************/
static void handle_close(int iSignNo)
{
	//destroy_server(server);
	printf("捕捉到信号\n");
	exit(0);
}


/**********************************************
	服务器监听器
	当有数据来 		启动handler线程处理
	当断开连接时	删除和释放内存空间
***********************************************/

//void *server_listener(void *arg){
static void server_listener(void *arg){

	SERVER *server=(SERVER *)arg;
	struct epoll_event events[MAXEVENTS]; //epoll最大事件数,容器

	while(true){	
			//被改变值时退出循环
			//等待内核通知，获取可读的fd
		int number=epoll_wait(server->efd,events,MAXEVENTS,-1);
		if(number < 0)
		{
			printf("epoll failure\n");
			break;
		}
		int i;
		for(i=0;i<number;i++){                     //遍历epoll的所有事件
			int sockfd=events[i].data.fd;        //获取fd
			if(sockfd == server->listenfd){      //有客户端建立连接

				handle_accept_event(server);

			}else if(events[i].events & EPOLLIN){      //efd中有fd可读,

				handle_readable_event(server,events[i]);

			}else if( events[i].events&EPOLLOUT ){      //efd中有fd可写

				handle_writeable_event();

			}else if(events[i].events&EPOLLPRI){
				printf("带外数据\n");
			}else{  //其他
				printf("其他错误\n");
			}
		}
	}
	pthread_exit(NULL);
	printf("监听线程退出\n");

}


/**************************

        初始化服务器端口

***************************/
void  init_server(SERVER **server,int port,SERVER_TYPE type){

	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=INADDR_ANY; 

	int ret=bind(sfd,(struct sockaddr *)&addr,sizeof(addr));  //绑定到服务器的端口
	if(ret == -1){
		printf("绑定到服务器的端口失败\n");
		close(sfd);
		return ;
	}
	
	ret=listen(sfd,BACKLOG);               //监听端口

	assert(ret != -1);

	server_set_sock(sfd);           	  //服务器套接字文件描述符

    int efd=epoll_create(MAXCONNS);        //创建epoll事件监听

    addfd(efd,sfd);         			   //注册TCP socket 上可读事件

    /**初始化客户端连接的数量**/
    *server=(SERVER *)malloc(sizeof(struct sock_server));
	(*server)->listenfd=sfd;    
	(*server)->connect_num=0; 			   
	(*server)->efd=efd;
	(*server)->tpool=threadpool_init(THREAD_NUM,TASK_QUEUE_NUM); //初始化线程池
	(*server)->run=true; //初始化线程池
	(*server)->conn_root=RB_ROOT;
	(*server)->type=type;
	
	/**注册监听信号进程**/
	signal(SIGKILL,handle_close);
    signal(SIGINT,handle_close);
	signal(SIGTERM,handle_close);
	printf("init sucesss!\n");

}

/*******************************
	开始监听服务器的连接
******************************/
void  start_listen(SERVER *server){
	pthread_t pt;
	pthread_create(&pt,NULL,(void *)&server_listener,(void *)server);
	//pthread_detach(pt);
	pthread_join(pt,NULL);
}

/************************
*关闭服务器器监听
*************************/
void  destroy_server(SERVER *server)
{
	/****删除所有连接节点****/

	deletefd(server->efd,server->listenfd);

	close(server->listenfd);
	close(server->efd);
	threadpool_destroy(server->tpool);

	free(server);
	printf("销毁监听\n");
}