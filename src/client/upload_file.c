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
#include "wfs_util_func.h"
#include "global_config.h"
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

static
void buff_to_block(int fd,
	unsigned char *buff,
	struct  file_block_queue *item,
	int len,
	unsigned long id,
	char *filename)
{

	item->block.version= (uint8_t)VERSION;
	item->block.type=    (uint8_t)TYPE_DATA;
	item->block.code=    (uint8_t)CODE_ZLIB;
	item->block.data_len=(uint32_t)len;
	item->block.checksum=checksum(buff,len);
	item->block.pkt_id = time(NULL); //返回从1970年1月1日至今的秒数。

	/*将数据包hash到服务器节点上*/
	unsigned char digest[16];
	MD5Buffer(buff,strlen(buff),digest);
	int idx=DJBhash(digest)%slave_server_num;
	char dest_addr[15];
	strcpy(dest_addr,slave_ip[idx]);
	//发送到的IP地址  ???
	item->block.to_addr.sin_family=AF_INET;
	item->block.to_addr.sin_addr.s_addr = inet_addr(slave_ip[idx]); /*本地地址*/
	item->block.to_addr.sin_port = htons(g_port); 		/*监听端口*/

	memcpy(item->block.buff,buff,len);
	
	item->block.block_flag=(struct ugi *)malloc(sizeof(struct ugi));
	if(item->block.block_flag == NULL)
		errExit("file:%s,line:%d \n",__FILE__,__LINE__);

	struct stat file_stat;  
	fstat(fd,&file_stat); 			//获取文件的各种信息

	item->block.block_flag->filesize = file_stat.st_size;
	item->block.block_flag->stime    = file_stat.st_mtime;
	strcpy(item->block.block_flag->filename , filename);    	  //数据包所属的文件名 ???
	item->block.block_flag->block_id  = id;         			  //数据包的id  ???
	item->block.block_flag->offset    = id*WFS_FILE_BUFF_SIZE;    //数据包在文件中的偏移量 
	//data_save_path数据存储的路径
	strcpy(item->block.block_flag->block_path,data_save_path);    //文件存储路径??

}


void upload_file(char *path,char *filename)
{
	TAILQ_INIT(&file_block_queue); //初始化队列

	int fd=open(path,O_RDONLY);
	if(fd < 0)
	{
		errExit("open file error.file:%s,line:%d,funciton:%s\n",__FILE__,__LINE__,__FUNCTION__);
	}
	
	int ret;
	//WFS_FILE_BUFF_SIZE BUFF长度 1024*5
	unsigned char buff[WFS_FILE_BUFF_SIZE];

	unsigned long count_id=0;

	char file_name[100];

	gname_frpath(path,file_name);	//从路径中获取文件名

	while((ret=read(fd,buff,WFS_FILE_BUFF_SIZE)) >= 0)
	{
		struct  file_block_queue item;
		buff_to_block(fd,buff,&item,ret,count_id,file_name);//将buff数据放入队列中
		
		//将数据包添加到队列中
		TAILQ_INSERT_TAIL(&file_block_queue, &item, next);    // 在尾部添加

		count_id++;
	}

	close(fd);
}