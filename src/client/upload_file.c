#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>

#include "upload_file.h"
#include "queue.h"
#include "zlib.h"
#include "error_hdr.h"
#include "util.h"
#include "config.h"
#include "hash.h"
#include "md5.h"

/************************************
	将buf封装到buf_to_block

	@param
		fd {int} 打开的文件描述符
	@param
		buf {unsigned char} 待封装的buf
	@param
		itme {struct  file_block_queue} 队列节点
	@param
		len {int}  buf长度
	@param
		id 	{unsigned long}  buff对应文件的id
	@return
		void
***********************************************/

static void gname_frpath(const char *path,char *name)
{
	int index=-1;
	int i=0;

	while(path[i] != '\0')
	{
		if(path[i] == '/')
		{
			index=i;
		}
		i++;
	}

	if(index == -1)
	{
		strcpy(name,path);
		return;
	}

	int j=0;
	for(i=index+1;path[i]!='\0';i++,j++)
	{
		name[j]=path[i];
	}
}
