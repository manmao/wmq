#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "error_hdr.h"
#include "init.h"
#include "log.h"
#include "slave_init.h"
#include "master_init.h"
#include "config.h"
#include "test.h"

void  process(int argc , char *argv[]);

/**
   start master:
     ./bin/wfs master 1

   start slave:
     ./bin/wfs slave  1
     ./bin/wfs slave  2
     ./bin/wfs slave  3
     ...
     ./bin/wfs slave  n
**/
void child_run(int argc,char *argv[])
{
	if(argc<3)
	{
	 	printf("参数有误--请输入参数:\n");
	 	printf(" master 1 启动master服务器\n");
	 	printf(" slave  x 启动slave服务器\n");
	}

	//启动master服务器
	if(strcmp(argv[1],"master") == 0)
	{
	 	master_server_init(0,NULL);
	 	printf("master_server_init started....\n");
	}

	//启动slave服务器
	else if(strcmp(argv[1],"slave") == 0)
	{
	 	slave_server_init(argc,argv);
	 	printf("slave_server_init started....\n");
	}
	//参数有误
	else
	{
	 	printf("参数有误--请输入参数:\n");
	 	printf(" xxx  master 1 启动master服务器\n");
	 	printf(" xxx  slave  n 启动slave服务器\n");
	}

	while(1);

}


/**
*
*
*
*
*/
void parent_run(int argc,char *argv[])
{
	int status;
	pid_t ret;
	ret = wait(&status);   //wait

	if(ret <0){
		perror("wait error");
		exit(EXIT_FAILURE);
	}

	//exit normal
	if (WIFEXITED(status)){
		log_write(CONF.lf,LOG_INFO,"child exited normal exit status=%d\n",WEXITSTATUS(status));
	}

	//exit signal
	else if (WIFSIGNALED(status)){
		log_write(CONF.lf,LOG_ERROR,"child exited abnormal signal number=%d\n", WTERMSIG(status));
		log_write(CONF.lf,LOG_ERROR,"*************Sever Exception Exit!!!!************\n");
	}

	//exit un normal
	else if (WIFSTOPPED(status)){
		log_write(CONF.lf,LOG_ERROR,"child stoped signal number=%d\n", WSTOPSIG(status));
		log_write(CONF.lf,LOG_ERROR,"********************Sever Exception Exit!!!!**************\n");
		//child_run(argc,argv);
		process(argc,argv);
	}
}


void  process(int argc , char *argv[])
{
	pid_t mainpro;

	/*init*/
	init_log();
	init_conf();

	mainpro = fork();
	if(mainpro <= -1)
	{
		errExit("fork失败,file:%s,line:%d",__FILE__,__LINE__);
	}
	if(mainpro > 0) //父进程
	{
		parent_run(argc,argv);
	}
	else if(mainpro == 0) //子进程
	{
		child_run(argc,argv);
	}
}

/*********************************

	整个系统入口

**********************************/

#if 1

int main(int argc , char *argv[])
{
	process(argc,argv);
	return 0;
}

#endif
