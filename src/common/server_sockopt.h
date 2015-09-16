#ifndef _SERVER_SOCKOPT_H_
#define _SERVER_SOCKOPT_H_

#include <sys/socket.h>

#include "common_define.h"
#include "queue.h"
#include "threadpool.h"

#define IP_SIZE 		20        // ip的 长度
#define BACKLOG  	  	10        // 服务器侦听长度

#define MAXCONNS 		65535	  // 服务器最大连接数
#define MAXEVENTS       100		  // 最大事件数

#define THREAD_NUM       10	      // 线程池开启的线程个数
#define TASK_QUEUE_NUM   10000    // 队列的最大job个数 

/******保存客户端连接的节点******/
typedef struct connection{
	int accept_fd;           							 	//和客户端连接的文件描述符
	struct sockaddr clientaddr;  					 	    //客户端地址
	int (*do_task)(void *arg);  	 	//回调函数，处理连接
	TAILQ_ENTRY(connection) next;
}CONNECT;


/****服务器结构****/
typedef struct sock_server{
	int listenfd; 									 //服务端监听listenfd
	int efd;										 //epoll文件描述符
	int connect_num;         						 //连接数据量
	struct threadpool *tpool;				 		 //线程池
	TAILQ_HEAD(connects, connection) ConnectQueue;   //保存客户端连接队列
	bool run;
}SERVER;

extern void  init_server(SERVER **server,int port);	//初始化
extern void  start_listen(SERVER *server);  		//开启服务器监听

extern void  handle_request(struct connection *code);
extern void  destroy_server(SERVER *server);
extern void  server_set_sock(int sfd);				//设置套接字选项




#endif