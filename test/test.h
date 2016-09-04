#include <string.h>
#include "client_util.h"
#include "socket_pkg.h"


void client_recv_data1(struct sock_client *client){
	 char buff[1024];
	 //接收数据
	 while(1){
	 	int ret=recv(client->conn->fd,buff,1024, 0);
	 	if(ret >0)
	 		printf("%s\n",buff);
	}
	return buff;
}


int test_client(int argc,char *argv[]){

	struct sock_client *client;
	
	client_init(&client,"127.0.0.1",9090);

	start_connection(client);

	//注册主题
	struct socket_pkg *pkg=create_socket_pkg_instance();
	pkg->type= 0x03;
	pkg->cmd=0x0003;
	strcpy(pkg->msg.topic,"top1");
	client_send_data(client,pkg);

	//发送消息
	struct socket_pkg *pkg1=create_socket_pkg_instance();
	pkg1->type= 0x03;
	pkg1->cmd=0x0004;
	strcpy(pkg1->msg.msg_buff,"helle wmq hhhhh");
	strcpy(pkg1->msg.topic,"top1");

	client_send_data(client,pkg1);

	client_recv_data1(client);
	

	while(1);

	return 0;
}