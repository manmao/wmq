/************************************************

	本头文件主要用来存放一些系统的全局变量
	这些变量通过系统初始化的时候，从配置文件中读取出来

*************************************************/

#ifndef __GLOBAL_CONFIG_H
#define __GLOBAL_CONFIG_H


typedef struct system_conf
{
	int master_server_num;

	//master
 	char master_ip[20];

	//DataNode集群服务器数量
	int slave_server_num;

	//slave
 	char *slave_ip[20];

	//文件在数据节点上存储的路径
	char *data_save_path;

	//slave监听端口
	int slave_port;

	//
	int master_port;

	
	FILE *lf;

	//文件系统存储路径
	char *logpath;

}SYSTEM_CONF;

SYSTEM_CONF CONF;

#endif