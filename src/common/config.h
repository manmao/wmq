/************************************************
	本头文件主要用来存放一些系统的全局变量
	这些变量通过系统初始化的时候，从配置文件中读取出来
*************************************************/

#ifndef __GLOBAL_CONFIG_H
#define __GLOBAL_CONFIG_H

#include <stdio.h>

typedef struct system_conf
{
	// Log file
	FILE *lf;
	//log file path
	char *logpath;
	//消息队列个数
	int queue_num=4;
	
}system_conf_t;

typedef struct system_net_conf{
	char *ip;
	int port;
}system_net_conf_t;


//系统配置全局变量
extern system_conf_t CONF;


//网络配置全局变量
extern system_net_conf_t NET_CONF;

#endif