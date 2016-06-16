#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "socket_pkg.h"
#include "config.h"
#include "log.h"

#include "server_init.h"


server_t *master_server=NULL;

static
void handle_sig(int sig)
{
    printf("catch sig %d\n",sig);
    exit(-1);
}

static
void on_accept(int accept_fd)
{

}

static
int on_handle(struct conn_node *node)
{
    //将数据包加入任务队列
    //放入线程池
    threadpool_add_job(master_server->tpool,(void *)&handle_request,node);
    return 0;
}


int server_init(int argc,char *argv[])
{
    //挂接服务器事件处理函数
    struct server_handler *handler=(struct server_handler *)malloc(sizeof(struct server_handler));

    //如果没有相关接口实现的，一定要赋值为空值
    handler->handle_readable=&on_handle;
    handler->handle_accept=&on_accept;
    handler->handle_unknown=NULL;
    handler->handle_sig=&handle_sig;

    start_listen(master_server,10,10000); //启动服务器监听子进程
    return 0;
}


void handle_request(void *arg){
   struct conn_node *node=(struct conn_node *)arg;
   socket_pkg_t *socket_pkt_ptr=NULL;  
   while(1)
   {
       socket_pkt_ptr =(struct socket_pkg*)malloc(sizeof(struct socket_pkg));
       socket_pkt_ptr->msg=(message_t *)malloc(sizeof(message_t ));

       assert(socket_pkt_ptr != NULL);
       assert(socket_pkt_ptr->msg != NULL);

       int buflen=recv(node->conn_fd,(void *)socket_pkt_ptr,sizeof(socket_pkg_t),0);

       if(buflen < 0)
       {
           //读取完成
           if(errno== EAGAIN || errno == EINTR){ 
               log_write(CONF.lf,LOG_INFO,"no data:file:%s,line :%d\n",__FILE__,__LINE__);

           }else{
               log_write(CONF.lf,LOG_INFO,"error:file:%s,line :%d\n",__FILE__,__LINE__);                            //error
           }
           free(socket_pkt_ptr->msg);
           free(socket_pkt_ptr);
           return -1;
       }
       else if(buflen==0)           
       {
          //删除连接节点
          conn_delete(&master_server->conn_root,&node);

          free(socket_pkt_ptr->msg);
          free(socket_pkt_ptr);
          return ;
       }
       else if(buflen>0)
       {
          log_write(CONF.lf,LOG_INFO,"%s","data comming....\n");
       }
   }
}

