#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "config_util.h"
#include "config.h"
#include "log.h"

/*************************
*
* 初始化日志文件系统
*
***************************/

void init_log()
{
	char KeyVal[100];
	get_config_string("config/wfs_config.conf","wfs","logFilePath",KeyVal);
	CONF.lf=log_file_open(KeyVal);
	log_write(CONF.lf,LOG_INFO,"log file path : %s\n",KeyVal);
	
}

/*****************************
*
* 初始化全局变量，从配置文件中读取
*
*****************************/


void init_conf(void)
{
	
}