/************************************************
	本头文件主要用来存放一些系统的全局变量
	这些变量通过系统初始化的时候，从配置文件中读取出来
*************************************************/

#ifndef __GLOBAL_CONFIG_H
#define __GLOBAL_CONFIG_H


typedef struct system_conf
{

	// Log file
	FILE *lf;

	//文件系统存储路径
	char *logpath;

}system_conf_t;

system_conf_t CONF;


#endif