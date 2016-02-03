#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "common_define.h"
#include "global.h"

#include "server_sockopt.h"
#include "master_init.h"

static
void handle_sig(int sig)
{
    printf("catch sig %d\n",sig);
    exit(-1);
}

static
void on_master_accept(int accept_fd)
{
    //放入线程池
}


static
int on_master_handle(struct request *req_pkg_p)
{
    //将数据包加入任务队列
    //threadpool_add_job(server->tpool,handle_pkg,(void *)&recv_pkt);
    //printf("包个数: ==> %d\n",count++);
    //往线程池添加执行单元
    //放入线程池
    return 0;
}

static struct sock_server *master_server=NULL;

int master_server_init(int argc,char *argv[])
{
    //挂接服务器事件处理函数
    struct server_handler *handler=(struct server_handler *)malloc(sizeof(struct server_handler));
    //如果没有相关接口实现的，一定要赋值为空值
    handler->handle_readable=&on_master_handle;
    handler->handle_accept=NULL;
    handler->handle_unknown=NULL;
    handler->handle_writeable=NULL;
    handler->handle_urg=NULL;
    handler->handle_sig=&handle_sig;
    
    init_server(&master_server,CONF.master.port,handler,50,10000);
    start_listen(master_server); //启动服务器监听子进程
    return 0;
}
