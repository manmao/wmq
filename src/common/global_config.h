/************************************************

	本头文件主要用来存放一些系统的全局变量
	这些变量通过系统初始化的时候，从配置文件中读取出来

*************************************************/

#ifndef __GLOBAL_CONFIG_H
#define __GLOBAL_CONFIG_H


//DataNode集群服务器数量
static int slave_server_num;

//master
static char master_ip[20];

//slave
static char *slave_ip[20];

///本机的IP地址

//文件在数据节点上存储的路径
static char *data_save_path;

//slave监听端口
static int g_port;

//日志系统流句柄
static FILE *lf; //全局变量

//文件系统存储路径
static char *logpath;

#endif