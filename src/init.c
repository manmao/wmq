#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "global.h"
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
	GetConfigStringValue("config/wfs_config.conf","wfs","logFilePath",KeyVal);
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
	int num;
	GetConfigIntValue("config/wfs_config.conf","slave","NUM",&num);
	printf("%d\n",num);

	CONF.slave_server_num=num;
	CONF.slave=(struct addr *)malloc(sizeof(struct addr)*num);
	int i;
	for(i=1;i<=num;i++){

		char tmp[10];
		sprintf(tmp,"IP%d",i);
		GetConfigStringValue("config/wfs_config.conf","slave",tmp,CONF.slave[i-1].ip);
		sprintf(tmp,"PORT%d",i);
	    GetConfigIntValue("config/wfs_config.conf","slave",tmp,&CONF.slave[i-1].port);
	    log_write(CONF.lf,LOG_INFO,"slave:%s:%d\n",CONF.slave[i-1].ip,CONF.slave[i-1].port);
	}

	//====master
	CONF.master_server_num=1;
	GetConfigStringValue("config/wfs_config.conf","master","IP",CONF.master.ip);
	GetConfigIntValue("config/wfs_config.conf","master","PORT",&CONF.master.port);
	log_write(CONF.lf,LOG_INFO,"master: %s:%d\n",CONF.master.ip,CONF.master.port);

	//====data save path
	CONF.data_save_path=(char *)malloc(sizeof(char)*100);
	GetConfigStringValue("config/wfs_config.conf","wfs","dataPath",CONF.data_save_path);
	printf("%s\n",CONF.data_save_path);


}