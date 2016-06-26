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
#include "server.h"
#include "config.h"
#include "sig_handler.h"

void  process_init(int argc , char *argv[]);

void child_run(int argc,char *argv[])
{
	server_init(argc,argv);
	while(1);

}


/**
 * [parent_run description]
 * @param argc [description]
 * @param argv [description]
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
		log_write(CONF.lf,LOG_ERROR,"*************Server Exception Exit!!!!************\n");
	}

	//exit un normal
	else if (WIFSTOPPED(status)){
		log_write(CONF.lf,LOG_ERROR,"child stoped signal number=%d\n", WSTOPSIG(status));
		log_write(CONF.lf,LOG_ERROR,"**************Server Exception Exit!!!!********\n");
		log_write(CONF.lf,LOG_ERROR,"File:%s ,Line:%d\n",__FILE__,__LINE__);
		//child_run(argc,argv);
		process_init(argc,argv);
	}
}

/**
 * 系统进程初始化
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 */
void  process_init(int argc , char *argv[])
{
	pid_t mainpro;
	char config_path[512];
	if(argc<2)
		strcpy(config_path,"./config/wfs_config.conf");
	else
		strcpy(config_path,argv[1]);
	

	/*init*/
	init_log(config_path);

	//注册信号处理函数，当程序异常退出时，打印堆栈信息
	registe_sig_handler(); 
	
	//init config
  	init_conf(config_path);

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
	process_init(argc,argv);
	return 0;
}

#endif
