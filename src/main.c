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
#include "server_init.h"
#include "config.h"

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

	server_init(argc,argv);
	
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
