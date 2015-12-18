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
	CONF.lf=logFileOpen(KeyVal);
	printf("%s\n",KeyVal);
   //logFileClose(lf); //记得注释掉
}

/*****************************
*
* 初始化全局变量，从配置文件中读取 
*
*****************************/
void init_conf(void)
{
	char ip_tmp[10][20];
	get_ip("config/slave",ip_tmp,&CONF.slave_server_num);
	printf("slave num:%d\n ",CONF.slave_server_num);

	int i;
	for(i=0; i<CONF.slave_server_num;i++)
	{
		CONF.slave_ip[i]=(char *)malloc(sizeof(char)*20);
		strcpy(CONF.slave_ip[i],ip_tmp[i]);
	}

	int master_server_num;
	get_ip("config/master",ip_tmp,&CONF.master_server_num);
	strcpy(CONF.master_ip,ip_tmp[0]);
	logWriter(CONF.lf,LOG_INFO,"master ip:%s\n\n",CONF.master_ip);

	CONF.data_save_path=(char *)malloc(sizeof(char)*100);
	GetConfigStringValue("config/wfs_config.conf","wfs","dataPath",CONF.data_save_path);
	printf("%s\n",CONF.data_save_path);

	GetConfigIntValue("config/wfs_config.conf","wfs","slave_port",&CONF.slave_port);
	printf("%d\n",CONF.slave_port); 

	GetConfigIntValue("config/wfs_config.conf","wfs","master_port",&CONF.master_port);
	printf("%d\n",CONF.master_port); 

 
}