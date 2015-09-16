#include "wfs_util_func.h"
#include <fcntl.h>
#include <unistd.h>

/*************************
 	求一个buff的ip校验和
 @param
 	buff   待求ip校验和的buff
 @param
		len 	buff的长度
 @return 
 		返回ip校验和
**************************/
extern unsigned short checksum(unsigned char *buf,int len){

	unsigned int sum=0;
	unsigned short *cbuf;
	cbuf=(unsigned short *)buf;
	while(len>1){
		sum+=*cbuf++;
		len=len-2;
	}
	if(len) /*长度为奇数*/
		sum=sum+*((unsigned char *)cbuf);

	while(sum>>16){
		sum=(sum >> 16 )+(sum & 0xffff);
	}
	return ~sum;  /*返回校验和*/
}

/***************************
 	设置文件描述符为非阻塞
 @param	
		fd   文件描述符
 @param
		len 	buff的长度
 @return  int
 		返回标志 
****************************/
void setnonblock(int fd)
{
  	int flags;
 	 flags = fcntl(fd, F_GETFL);
  	flags |= O_NONBLOCK;
  	fcntl(fd, F_SETFL, flags);
}

/********************************
	判断两个unsigned char buff值是否相等
	
@param 	
		{unsigned char} buf1  需要比较的buff1	
@param
		{unsigned char} buf2  需要比较的buff2
@return 
		{int}   如果相等 返回0 如果不相等返回 -1 

********************************/
int is_equal(unsigned char *buf1,unsigned char *buf2)
{
	while(*buf1 != '\0' && *buf2 != '\0')
	{
		if(*buf1 != *buf2)
		{
			return -1;

		}
		buf2++;
		buf1++;
	}

	return 0;
}