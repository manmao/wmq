#ifndef __CONNECT_H_
#define __CONNECT_H_

#include <sys/socket.h>

#include "rbtree.h"
#include "util.h"
#include "server_util.h"

/******保存客户端连接的节点******/
struct conn_node{
	int conn_fd;          			//和客户端连接的文件描述符
    int epoll_fd;         			//epoll内核事件表指针
    struct sockaddr clientaddr;  	//客户端地址
    char *topic;					//注册的主题
    void (*callback)(void *item);	//回调函数
};

/****** rbtree 节点******/
struct conn_type {

    struct rb_node my_node;

    struct conn_node *node;
};

struct conn_type *conn_search(struct rb_root *root, struct conn_node *conn);

int conn_insert(struct rb_root *root, struct conn_type *data);

void conn_delete(struct rb_root *root, struct conn_node *conn);

void print_rbtree(struct rb_root *tree);

#endif