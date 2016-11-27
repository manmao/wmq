#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "config_util.h"
#include "config.h"
#include "log.h"

/*************************
*
* 初始化日志文件系统
*
***************************/
void init_log(char *config_path)
{
	char KeyVal[100];
	get_config_string(config_path,"wmq","logFilePath",KeyVal);
	CONF.lf=log_file_open(KeyVal);
	log_write(CONF.lf,LOG_INFO,"log file path : %s\n",KeyVal);
}


/*****************************
*
* 初始化全局变量，从配置文件中读取
*
*****************************/
void init_conf(char *config_path)
{
	NET_CONF.ip=(char *)calloc(sizeof(char)*20,'0');
	assert(NET_CONF.ip!=NULL);
	get_config_string(config_path,"network","ip",NET_CONF.ip);
	get_config_int(config_path,"network","port",&(NET_CONF.port));
	//获取消息队列个数
	get_config_int(config_path,"wmq","queueNum",&(CONF.queue_num));

}