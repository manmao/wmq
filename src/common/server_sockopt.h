#ifndef _SERVER_SOCKOPT_H_
#define _SERVER_SOCKOPT_H_

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>

#include "common_define.h"

#include "rbtree.h"
#include "threadpool.h"
#include "connect.h"

#define IP_SIZE 		20        // ip的 长度
#define BACKLOG  	  	5        // 服务器侦听长度

#define MAXCONNS 		65535	  // 服务器最大连接数
#define MAXEVENTS       100		  // 最大事件数

#define THREAD_NUM       10	      // 线程池开启的线程个数
#define TASK_QUEUE_NUM   10000    // 队列的最大job个数

/****服务器结构****/
typedef struct sock_server{
	int listenfd; 					 //服务端监听listenfd
	int efd;						 //epoll文件描述符
	int connect_num;         		 //连接数据量
	struct threadpool *tpool;		 //线程池
	struct rb_root    conn_root; 	 //客户端节点
	bool run;
	struct server_handler *handler;  //连接处理函数回调
}SERVER;

/**
*
*
*
*/
struct server_handler{
	int (*handle_accept)(struct sock_server *server,struct epoll_event event);
	int (*handle_readable)(struct sock_server *server,struct epoll_event event);
    int (*handle_writeable)(struct sock_server *server,struct epoll_event event);
    int (*handle_urg)(struct sock_server *server,struct epoll_event event);
    int (*handle_unknown)(struct sock_server *server,struct epoll_event event);
};


extern void  init_server(SERVER **server,int port,struct server_handler *handler);	//初始化

extern void  start_listen(SERVER *server);  		//开启服务器监听

extern void  destroy_server(SERVER *server);

extern void  server_set_sock(int sfd);				//设置套接字选项

/**
*
*epoll fd 操作
*/
extern void addfd(int epollfd,int fd);

extern void deletefd(int epollfd,int fd);

extern void modfd(int epollfd,int fd,int ev);

#endif