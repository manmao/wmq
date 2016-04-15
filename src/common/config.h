/************************************************
	本头文件主要用来存放一些系统的全局变量
	这些变量通过系统初始化的时候，从配置文件中读取出来
*************************************************/

#ifndef __GLOBAL_CONFIG_H
#define __GLOBAL_CONFIG_H

struct addr
{
	char ip[20];
	int port;
};

typedef struct system_conf
{
	//master
	int master_server_num;

 	struct addr master;

 	//slave
 	struct addr this_slave;

	//文件在数据节点上存储的路径
	char *data_save_path;

	// Log file
	FILE *lf;

	//文件系统存储路径
	char *logpath;

}SYSTEM_CONF;


typedef struct slaves{

	//DataNode集群服务器数量
	int slave_server_num;
	
	//cluster slave
 	struct addr *slave;

}SLAVES;


SYSTEM_CONF CONF;


#endif