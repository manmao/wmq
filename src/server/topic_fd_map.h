#ifndef __TOP_FD_MAP_H_
#define __TOP_FD_MAP_H_

#include <limit.h>

#include "hashtable.h"
#include "list.h"

struct list_entry{  //entry / elem
	char *topic;
	int  fd;
	TGAP_LIST_ENTRY(list_entry) field;  		//指向下一个节点或者上一个节点
};

// TGAP_LIST_HEAD(FD_LIST,list_entry) fd_list; //声明一个头结点
// TGAP_LIST_HEAD_INIT(&fd_list);	 		  //初始化头结点

struct hash_node{ 
	//head 
	TGAP_LIST_HEAD(FD_LIST,list_entry) fd_list; //声明一个头结点
};

extern HashTable *ht;


#endif