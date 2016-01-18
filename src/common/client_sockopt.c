#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>

#include "common_define.h"
#include "client_sockopt.h"
#include "util.h"

/********************************
*设置和服务端连接套接字的属性
*	主要作用是：
*				开启keepalive属性
*				设置端口和地址可重用*
*				禁用Nagle算法
*  函数名:client_set_sock
*  函数参数：
*  		@param  client 客户端操作实体
*		@param  sfd    和服务器端连接的套接字文件描述符
* 函数返回:
*		@return  null
*********************************/

void  client_set_sock(struct sock_client *client,int sfd){
    
	//开启keepalive属性
	setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (void*)&client->keepAlive, sizeof(client->keepAlive));

	 //设置客户端超时探测
	setsockopt(sfd,SOL_TCP, TCP_KEEPIDLE, (void*)&client->keepIdle, sizeof(client->keepIdle));

	//发送探测包的间隔时间
	setsockopt(sfd, SOL_TCP, TCP_KEEPINTVL, (void*)&client->keepInterval, sizeof(client->keepInterval));

	 //发送探测重试次数
	setsockopt(sfd, SOL_TCP, TCP_KEEPCNT, (void*)&client->keepCount, sizeof(client->keepCount));

    /***设置端口和地址可重用**/
	int optval=1;
	int optlen=sizeof(optval);
	int err=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(char *)&optval,optlen);

	/*******禁用Nagle算法***********/
	optval=1;
	optlen=sizeof(optval);
	err=setsockopt(sfd,IPPROTO_TCP,TCP_NODELAY,(char *)&optval,optlen);

    //设置linger,关闭close后的延迟，不进入TIME_WAIT状态
    struct linger ling= {0, 0};
    if (setsockopt(sfd, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling))!= 0)
    {
        return ;
    }

}


/********************************
*初始化客户端连接的一些属性,开辟空间
*	主要作用是：
*				发送心跳信息的间隔和时间
*				malloc 创建client实体
*				malloc 创建服务器端的连接实体
*
*  函数名:      client_init
*  函数参数：
*  		@param  client 客户端操作实体
* 函数返回:
*		@return  null
*
*********************************/

void client_init(struct sock_client **client,char *server_ip,int port){

	//client实体对象
	(*client)=(struct sock_client *)malloc(sizeof(struct sock_client));

	(*client)->keepAlive = 1;    //开启keepalive属性
	(*client)->keepIdle=60; 	//如果在60秒内没有任何数据交互,则进行探测. 缺省值:7200(s)
	(*client)->keepInterval=5; //探测时发探测包的时间间隔为5秒. 缺省值:75(s)
	(*client)->keepCount=2;   //探测重试的次数. 全部超时则认定连接失效..缺省值:9(次)

	//连接
	(*client)->conn=(struct connection *)malloc(sizeof(struct connection)); //初始化连接

	int fd=socket(AF_INET,SOCK_STREAM,0);  //创建套接字

	client_set_sock(*client,fd);         //设置keepalive属性

	if(-1 == fd)
	{
		printf("create socket fail\n");
		return -1;
	}

    /**初始化连接**/
	(*client)->conn->fd=fd;           //socket文件描述
	(*client)->conn->protocol=0x01;   //协议类型
	(*client)->conn->is_conn=false;   //没有连接

	struct sockaddr_in *saddr=(struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	memset(&saddr,0,sizeof(saddr));  //
	saddr->sin_family=AF_INET;        //设置协议族
	saddr->sin_port=htons(port);  //设置端口
	saddr->sin_addr.s_addr=inet_addr(server_ip); //设置ip地址

	(*client)->conn->saddr=(struct sockaddr *)saddr;

}



/**********************************************************************
*连接到服务器端
*	主要作用是：
*				创建socket
*				初始化连接信息
*
*
*  函数名:start_connection
*  函数参数:
*
*		@param  client 客户端操作实体
*
* 函数返回:
*		@return  int
*			 0  成功
*			 -1 连接服务器失败
*
****************************************************************/
int start_connection(struct sock_client *client){

	client->conn->is_conn=true;   //建立连接

	//连接服务器
	int ret=connect(client->conn->fd,(struct sockaddr *)client->conn->saddr,sizeof(struct sockaddr));
	if(ret == -1)
	{
		printf("连接服务器失败\n");
		return -1;
	}
	setnonblock(client->conn->fd);  //设置fd为非阻塞

	return 0;
}





/**********************************************************************
*发送数据到服务端
*	主要作用是：
*			发送数据到服务端
*
*  函数名:client_send_data
*  函数参数:
*		@param  client 客户端操作实体
*		@param  sendpkt 发送的数据包
*
* 函数返回:
*		@return  int
*			 0  成功
*			 -1 发送失败
*
*  struct sock_pkt的数据结构
*  struct sock_pkt{
*	u8	version;       //版本
*	u8  type;          //传输数据包类型  0x01 0X02 0x03
*	u8	code;		   //编码
*	u16 checksum;      //数据校验和
*	u16 pkt_id;		   //包的编号
*	u16 data_len;      //数据实际的长度长度
*	u8 buff[WFS_FILE_BUFF_SIZE];   	  //数据存储结构
*	};
*
****************************************************************/
int  client_send_data(struct sock_client *client,struct sock_pkt *sendpkt){
		//发送数
		int ret=send(client->conn->fd, sendpkt,sizeof(struct sock_pkt), 0);
		if(ret == -1){
			printf("发送信息失败\n");
			return -1;
		}
		return 0;
}


/**********************************************************************
*接收数据
*	主要作用是：
*			接收数据
*
*  函数名:client_recv_data
*  函数参数:
*		@param  client 客户端操作实体
*		@param  revpkt 接收到的数据包存放的实体
*
* 函数返回:
*		@return  int
*			 0  成功
*			 -1 接收失败
*
*  struct sock_pkt的数据结构
*  struct sock_pkt{
*	u8	version;       //版本
*	u8  type;          //传输数据包类型  0x01 0X02 0x03
*	u8	code;		   //编码
*	u16 checksum;      //数据校验和
*	u16 pkt_id;		   //包的编号
*	u16 data_len;      //数据实际的长度长度
*	u8 buff[WFS_FILE_BUFF_SIZE];   	  //数据存储结构
*	};
*
****************************************************************/
int client_recv_data(struct sock_client *client,  struct sock_pkt *revpkt){
	 //接收数据
	 int ret=recv(client->conn->fd,revpkt,sizeof(struct sock_pkt), 0);
	 if(ret == -1){
			printf("接收信息失败\n");
			return -1;
	}
	return 0;
}



/**********************************************************************
*	关闭连接
*	主要作用是：
*			关闭连接文件描述符  close(client->conn->fd);
*
*  函数名:destroy_connection
*  函数参数:
*		@param  client 客户端操作实体
*
* 函数返回:
*		@return null
*
*
****************************************************************/
void destroy_connection(struct sock_client *client){
	close(client->conn->fd);     //关闭连接
	client->conn->is_conn=false;
}

/************************************
	销毁客户端实体
*************************************/
void client_destroy(struct sock_client *client){
	free(client->conn);
	free(client);
}



