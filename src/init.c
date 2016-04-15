#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
	
	//====//master
	CONF.master_server_num=1;
	get_config_string("config/wfs_config.conf","master","ip",CONF.master.ip);
	get_config_int("config/wfs_config.conf","master","port",&CONF.master.port);
	log_write(CONF.lf,LOG_INFO,"master: %s:%d\n",CONF.master.ip,CONF.master.port);

	//slave
	get_config_string("config/wfs_config.conf","slave","ip",CONF.this_slave.ip);
	get_config_int("config/wfs_config.conf","slave","port",&CONF.this_slave.port);
	log_write(CONF.lf,LOG_INFO,"slave: %s:%d\n",CONF.this_slave.ip,CONF.this_slave.port);


	//====//data save path
	CONF.data_save_path=(char *)malloc(sizeof(char)*100);
	get_config_string("config/wfs_config.conf","wfs","dataPath",CONF.data_save_path);
	printf("%s\n",CONF.data_save_path);


}