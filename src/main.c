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
#include "global.h"

#include "test.h"

void child_run(int argc,char *argv[])
{
	if(argc<2)
	 {
	 	printf("参数有误--请输入参数:\n");
	 	printf("-master 启动master服务器\n");
	 	printf("-slave 启动slave服务器\n");
	 }

	 //启动master服务器
	 if(strcmp(argv[1],"master") == 0)
	 {
	 	master_server_init(0,NULL);
	 	printf("master_server_init started....\n");
	 }

	 //启动slave服务器
	 else if( strcmp(argv[1],"slave") == 0 )
	 {
	 	slave_server_init(0,NULL);
	 	printf("slave_server_init started....\n");
	 }

	 //参数有误 
	 else
	 {
	 	printf("参数有误--请输入参数:\n");
	 	printf("xxx -master 启动master服务器\n");
	 	printf("xxx -slave 启动slave服务器\n");
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
		logWriter(CONF.lf,LOG_INFO,"child exited normal exit status=%d\n",WEXITSTATUS(status));
	}

	//exit signal 
	else if (WIFSIGNALED(status)){
		logWriter(CONF.lf,LOG_ERROR,"child exited abnormal signal number=%d\n", WTERMSIG(status));
	}

	//exit un normal
	else if (WIFSTOPPED(status)){
		logWriter(CONF.lf,LOG_ERROR,"child stoped signal number=%d\n", WSTOPSIG(status));
		child_run(argc,argv);
	}

}

/*********************************

	整个系统入口	

**********************************/

int main(int argc , char *argv[])
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

	return 0;
}
