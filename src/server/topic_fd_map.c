#include<string.h>
#include "topic_fd_map.h"

HashTable *ht;


HashTable *create_fdtopic_hashtable(){
	 HashTable *ht = create_hashtable(100,char*,long);
	 return ht;
}

void add_topic(HashTable *ht,char *topic,int fd){
	
	struct list_entry *entry=(struct list_entry*)malloc(sizeof(struct list_entry));
	entry->fd=fd;
	entry->topic=(char *)malloc(sizeof(strlen(topic)));
	strcpy(entry->topic,topic);

	struct hash_node *node=NULL;
	hash_find(ht,topic,node);//查询topic相关的fd列表
	if(node == NULL){//没有注册fd链表
		
		node=(struct hash_node *)malloc(sizeof(struct hash_node));
		//初始链表
		TGAP_LIST_HEAD_INIT(&(node->fd_list));
		//在链表尾部插入节点
		TGAP_LIST_INSERT_TAIL(&(node->fd_list),entry,field);
		
		//添加到hashtable
		hash_add(ht,topic,node); //添加到hash

	}else{//有注册fd得链表
		TGAP_LIST_INSERT_TAIL(&(node->fd_list),entry,field);
	}
}

struct hash_node *find_topic_fdlist(HashTable *ht,char *topic){
	struct hash_node *node=NULL;
	hash_find(ht,topic,node);//查询topic相关的fd列表
	return node;
}