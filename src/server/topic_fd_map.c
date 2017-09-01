#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "topic_fd_map.h"


HashTable *create_fdtopic_hashtable(){
	 HashTable *ht = create_hashtable(100,char*,long);
	 return ht;
}

void add_topic(HashTable *ht,
	char *topic,
	int fd,
	pthread_rwlock_t *ht_lock){

	struct list_entry *entry=(struct list_entry*)malloc(sizeof(struct list_entry));
	entry->fd=fd;
	entry->topic=(char *)malloc(sizeof(strlen(topic)));
	memcpy(entry->topic,topic,128);

	struct hash_node *node=NULL;
	
	pthread_rwlock_rdlock(ht_lock); //阻塞,加锁
	hash_find(ht,topic,&node);//查询topic相关的fd列表
	pthread_rwlock_unlock(ht_lock);

	if(node == NULL){//没有注册fd链表
		
		node=(struct hash_node *)malloc(sizeof(struct hash_node));

		//初始链表
		TGAP_RWLIST_HEAD_INIT(&(node->fd_list_head));
		//在链表尾部插入节点
		TGAP_RWLIST_WRLOCK(&(node->fd_list_head)); //加锁
		TGAP_LIST_INSERT_TAIL(&(node->fd_list_head),entry,field);
		TGAP_RWLIST_UNLOCK(&(node->fd_list_head)); //解锁

		pthread_rwlock_wrlock(ht_lock); //阻塞,加锁
		hash_add(ht,topic,node); //添加到hash
		pthread_rwlock_unlock(ht_lock);

	}else{//有注册fd得链表
		TGAP_RWLIST_WRLOCK(&(node->fd_list_head));
		TGAP_LIST_INSERT_TAIL(&(node->fd_list_head),entry,field);
		TGAP_RWLIST_UNLOCK(&(node->fd_list_head));
	}
}


struct hash_node *find_topic_fdlist(HashTable *ht,char *topic,pthread_rwlock_t *ht_lock){
	struct hash_node *node=NULL;
	pthread_rwlock_rdlock(ht_lock);              
	hash_find(ht,topic,&node);//查询topic相关的fd列表
	pthread_rwlock_unlock(ht_lock);
	return node;
}


void delete_fd(HashTable *ht,char *topic,int fd){

	struct hash_node *node=NULL;
	hash_find(ht,topic,&node);//查询topic相关的fd列表

	if(node == NULL) return;

	//delete fd
	struct list_entry *current;

	TGAP_LIST_TRAVERSE_SAFE_BEGIN( &(node->fd_list_head), current, field){
		if(current->fd == fd){
			TGAP_RWLIST_WRLOCK(&(node->fd_list_head));
			TGAP_LIST_REMOVE_CURRENT(field);
			TGAP_LIST_UNLOCK(&(node->fd_list_head));
		}
	}
	TGAP_LIST_TRAVERSE_SAFE_END
}


void destroy_fdtopic_hashtable(HashTable *ht){
	
	assert(ht!=NULL);

	for(reset(ht);isnotend(ht);next(ht)){
        char *t_key = skey(ht);
        long v_tmp = *(long *)value(ht);
        struct hash_node *node=v_tmp;
        if(node == NULL)
        	continue;
        //delete fd list
		struct list_entry *current;
        TGAP_LIST_TRAVERSE_SAFE_BEGIN( &(node->fd_list_head), current, field){
			TGAP_LIST_REMOVE_CURRENT(field);
		}
		TGAP_LIST_TRAVERSE_SAFE_END
		free(node);
	}

}