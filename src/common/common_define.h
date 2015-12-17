#ifndef _COMMON_COMMON_DEFINE_H
#define _COMMON_COMMON_DEFINE_H

#include <stdbool.h>  //C99 标准包含的头文件 包括bool
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include "util.h"

//自定义bool类型
//#define  bool int
//#define  true  1
//#define  false 0

#define IP_SIZE 20            	//ip的长度
#define WFS_FILE_BUFF_SIZE 1024*5  //pkt包数据的大小 

//版本号
#define VERSION    0x01

//数据包类型
#define TYPE_CMD   0X01
#define TYPE_DATA  0x02

//编码类型
#define CODE_LZMA  0x01
#define CODE_ZLIB  0x02

//监听端口
#define LISTERN_PORT    9998      // 服务器监听端口


//数据包标识，标识来自某个IP的某个文件
struct  ugi 
{
  //struct sockaddr_in  addr;		            //ip地址
	time_t 		        stime;					//文件最后一次修改时间
	unsigned long 	    filesize;				//文件大小
	char		  	    filename[50];				//所属的文件名
	
	unsigned long       block_id;				//数据块编号
	char 				block_path[100];			//存储的完整路径包括文件名
	unsigned int 		offset;					//数据包在文件的偏移量
};

/****************************
 socket 网络传输数据包
 		的定义 
 		头部和数据
****************************/
struct sock_pkt{ 
	uint8_t   version;       		 //协议版本
	uint8_t   type;          		 //传输数据类型  命令/数据(0x01/0x02)
	uint8_t	  code;		   		 	 //数据编码方式  lmza/zlib(0x01/0x02)
	uint32_t  data_len;      		 //数据长度
	uint16_t  checksum;      		 //数据校验和
	uint16_t  pkt_id;		   		 //数据包的编号(一个时间戳值)
	struct    sockaddr_in  to_addr;	 //发送到的目的IP地址
	struct    ugi          *block_flag;  		      //数据包标识
	unsigned char          buff[WFS_FILE_BUFF_SIZE]; //数据{5KB大小}
};

#endif