#ifndef __TOP_FD_MAP_H_
#define __TOP_FD_MAP_H_


#include "hashtable.h"
#include "lists.h"

struct list_entry{  //entry / elem
	char *topic;
	int  fd;
	TGAP_LIST_ENTRY(list_entry) field;  		//指向下一个节点或者上一个节点
};

// TGAP_LIST_HEAD(FD_LIST,list_entry) fd_list; //声明一个头结点
// TGAP_LIST_HEAD_INIT(&fd_list);	 		   //初始化头结点
struct hash_node{ 
	//head 
	TGAP_LIST_HEAD(FD_LIST,list_entry) fd_list_head; //声明一个头结点
};

extern HashTable *ht;

/**
 * 创建一个hashtable
 * @return [description]
 */
extern HashTable *create_fdtopic_hashtable();

/**
 * 添加topic和fd
 * @param ht    [hashtable]
 * @param topic [主题]
 * @param fd    [文件描述符]
 */
extern void add_topic(HashTable *ht,char *topic,int fd);

extern struct hash_node *find_topic_fdlist(HashTable *ht,char *topic);

extern void delete_fd(HashTable *ht,char *topic,int fd);

extern void destroy_fdtopic_hashtable(HashTable *ht);

#endif