#ifndef _SERVER_T_SOCKOPT_H_
#define _SERVER_T_SOCKOPT_H_

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <pthread.h>

#include "common_define.h"

#include "rbtree.h"
#include "threadpool.h"
#include "connect.h"

#include "msg_queue.h"

#include "topic_fd_map.h"

#define LISTEN_BACKLOG  	  	65535      // 服务器侦听长度

#define MAXCONNS 		65535	  // 服务器最大连接数
#define MAXEVENTS       100		  // 最大事件数

#define DEFAULT_THREAD_NUM       4	      // 线程池默认开启的线程个数
#define DEFAULT_TASK_QUEUE_NUM   50000    // 默认队列的最大job个数

#define DEFAULT_MESSAGE_QUEUE_NUM 20      //消息队列的个数

/****服务器结构****/
typedef struct server{
    
    int listenfd; 					 //服务端监听listenfd

	int efd;						 //epoll文件描述符

    struct threadpool *tpool;		 //线程池
    


    struct server_handler *handler;  //连接处理函数回调

    int queues;                  //消息队列的个数

    //mq群组 
    struct msg_queue *mq[DEFAULT_MESSAGE_QUEUE_NUM]; 

    HashTable *ht;                  //hash表，保存 topic-fd 列表的映射

    pthread_rwlock_t ht_lock;           //互斥锁

    struct rb_root   conn_root;      //客户端连接节点

    pthread_rwlock_t  rb_root_lock;      //读写锁

}server_t;

/**
*
*服务器端回调函数处理
*
*/
struct server_handler{

    //客户端连接事件
	int (*handle_accept)(int client_conn_fd,struct sockaddr clientaddr);
    //可读事件
    int (*handle_readable)(int readalbe_fd);
    //未知事件
    int (*handle_unknown)(int event_fd);
    //信号处理
    void (*handle_sig)(int sig);

    //启动监听
    int (*handle_listenmq)();
};

//初始化服务器
extern void  init_server(server_t *server,
                         char *ip,
                         int port,
                         struct server_handler *handler);	//初始化服务器

//开启服务
extern void  start_listen(server_t *server,
                         int thread_num,
                         int thread_queue_num);  		//开启服务器监听

extern void  destroy_server(server_t *server);

extern void  server_set_sock(int sfd);				//设置套接字选项

/**
*
*  epoll fd 操作
*
*/
extern void addfd(int epollfd,int fd);

extern void deletefd(int epollfd,int fd);

extern void modfd(int epollfd,int fd,int ev);

extern void set_nodelay(int fd);

#endif