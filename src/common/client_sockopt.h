#ifndef _CLIENT_SOCKOPT_H_
#define _CLIENT_SOCKOPT_H_

#include <sys/socket.h>
#include "common_define.h"

#define IP_SIZE 20            	//ip的长度


/***********************
保存和服务端的连接
************************/
struct connection{
	int fd;								//文件描述符 
	uint16_t protocol; 					//连接协议
	bool is_conn;						//连接是否关闭
	struct sockaddr *saddr;

};

/************************
和服务器建立连接的客户端实体对象
************************/
struct sock_client{

	int keepAlive;           // 开启keepalive属性. 缺省值: 0(关闭)  1(打开)

	int keepIdle;           //(单位:秒数)如果在60秒内没有任何数据交互,则进行探测. 缺省值:7200(s)  

	int keepInterval;       //(单位:秒数)探测时发探测包的时间间隔为5秒. 缺省值:75(s)

	int keepCount;   		 // 探测重试的次数. 全部超时则认定连接失效..缺省值:9(次)  

	struct connection *conn;  //服务器建立连接
	
};

extern void  client_set_sock(struct sock_client *client,int sfd);		//设置客户端keepalive

extern void client_init(struct sock_client **client,char *server_ip,int port);				//客户端初始化

extern int   start_connection(struct sock_client *client);       				//连接到服务器

extern int   client_send_data(struct sock_client *client, struct sock_pkt *sendpkt);//发送数据到服务器

extern int   client_recv_data(struct sock_client *client,  struct sock_pkt *revpkt);//接收返回数据

extern void  destroy_connection(struct sock_client *client);  					 	//释放连接

extern void  client_destroy(struct sock_client *client);  							//销毁客户端

#endif