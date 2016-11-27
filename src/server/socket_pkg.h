#ifndef __SOCKET_PKG_H
#define __SOCKET_PKG_H

#include <stdbool.h>  //C99 标准包含的头文件 包括bool
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>



#define MESSAGE_BUFF_SIZE 1024*5   //pkt包数据的大小

//版本号
#define VERSION    0x01

//数据包类型
#define TYPE_CMD   0X01 
#define TYPE_DATA  0x02
#define TYPE_CMD_DATA 0x03

//编码类型
#define CODE_GZIP     0x01  //gizp
#define CODE_JSON     0x02  //json
#define CODE_PROTOBUF 0x03  //protobuf
#define CODE_NATIVE   0x04  //默认


//cmd 命令
//打开消息通道
#define MQ_OPEN  0x0001  
//关闭消息通道
#define MQ_CLOSE 0x0002
//订阅主题
#define MQ_SUBTOPIC 0x0003
//发布消息
#define MQ_PUBMSG 0x0004


typedef struct socket_pkg{
	uint8_t            version;      //协议版本
	uint8_t	           code;         //数据编码方式  gzip/json/protobuf(0x01/0x02/0x03)
	int 			   fd;
	char 			   topic[255];   //topic
	uint16_t           cmd;          //数据包性质
	uint32_t           data_len;     //数据长度
	uint16_t           checksum;     //数据校验和

    uint8_t     	   msg[MESSAGE_BUFF_SIZE];		 //消息体body
}socket_pkg_t;


/**
 * 创建一个实例
 * @return [description]
 */
extern struct socket_pkg* create_socket_pkg_instance();

/**
 * 销毁一个实例
 */
extern void destroy_socket_pkg_instance(socket_pkg_t *socket_pkt_ptr);


#endif