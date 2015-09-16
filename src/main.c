#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>


#include "error_hdr.h"
#include "test.h"
#include "slave_init.h"
#include "master_init.h"
#include "init.h"

int    g_argc;	   //保存传入的参数  参数个数
char   *g_argv[];  //保存传入的参数  参数内容

//	系统进程退
void process_exit(int s)
{
	exit(0);
}

//子进程真正运行的方法
//进程运行方法
void main_fun(int argc,char *argv[])
{
	/*
	*根据参数设置相应的状态机
	*/

	init_log();
	init_conf();

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
	 	printf("-master 启动master服务器\n");
	 	printf("-slave 启动slave服务器\n");
	 }
	while(1);
}


//开辟子进程
void fork_child(int s)
{	
	pid_t child_process;
    int status;
    int signal_num;
    wait(&status);//等待子进程中断或终止，释放子进程资源，否则死掉的子进程会变成僵尸进程
    signal_num = WTERMSIG(status);

    child_process=fork();
    if(child_process == 0)
    {
    	printf("fork new child process.\n");
        main_fun(g_argc,g_argv);  ///功能函数入口
    }
}


/*********************************
	整个系统入口	
**********************************/
int main(int argc , char *argv[])
{
	pid_t mainpro;

	/****获取main函数参数*****/
	g_argc=argc;
	int i;
	for(i=1;i<=argc;i++)
		g_argv[i] = argv[i];
	/***************************/

	mainpro = fork();

	if(mainpro == -1)
	{
		errExit("fork失败,file:%s,line:%d",__FILE__,__LINE__);
	}

	else if(mainpro > 0) //父进程
	{
		while(1)
		{ 
			  //捕获子进程结束信号,如果子进程结束就会产生SIGCHLD信号
			  //当检测到SIGCHLD信号时，重新创建子进程
            //signal(SIGCHLD, fork_child); 
              //当接收到SIGTERM时,整个进程退出
              signal(SIGTERM, process_exit);
              pause();//主进程休眠，当有信号到来时被唤醒。
		}
	}
	else if(mainpro == 0) //子进程
	{
		 main_fun(argc,argv);
	}

	return 0;

}

#if 0
int main()
{
	testgetip();
	return 0;
}
#endif