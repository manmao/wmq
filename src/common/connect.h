#ifndef __CONNECT_H_
#define __CONNECT_H_


#include <sys/socket.h>

#include "rbtree.h"
#include "util.h"
#include "server_sockopt.h"


/******保存客户端连接的节点******/
struct conn_node{
	int accept_fd;           							 	//和客户端连接的文件描述符
	struct sockaddr clientaddr;  					 	    //客户端地址
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