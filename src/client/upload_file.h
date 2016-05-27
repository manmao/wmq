#ifndef __UPLOAD_FILE_H_
#define __UPLOAD_FILE_H_

#include "common_define.h"
#include "queue.h"

struct  file_block_queue{
	struct sock_pkt block;  	   //数据包
	TAILQ_ENTRY(file_block_queue) next; //
};

//数据队列
TAILQ_HEAD(queue, file_block_queue) file_block_queue;  

//上传文件到分布式存储系统
extern void upload_file(char *path,char *filename);

#endif