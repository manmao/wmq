#ifndef __TOP_FD_MAP_H_
#define __TOP_FD_MAP_H_
#include <limit.h>

#include "list.h"

struct node{
	int  fd;
	TAILQ_ENTRY(node) next;  //指向下一个节点或者上一个节点
};

TGAP_LIST_HEAD(queue,node) fd_list;//声明一个头结点
TGAP_LIST_HEAD_INIT(&fd_list);	 //初始化头结点

struct topic_fdlist{
	char *topic;
	struct node *fdlist;
};

extern struct topic_fdlist[INT_MAX];

#endif