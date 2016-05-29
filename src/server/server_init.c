#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "common_define.h"
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
    init_server(&master_server,CONF.master.ip,CONF.master.port,handler);

    start_listen(master_server,10,10000); //启动服务器监听子进程
    return 0;
}


void handle_request(void *arg){
   struct conn_node *node=(struct conn_node *)arg;
   struct request *req_pkt_p=NULL;  
   while(1)
   {
       req_pkt_p =(struct request*)malloc(sizeof(struct request));
       req_pkt_p->pkg=(struct sock_pkt *)malloc(sizeof(struct sock_pkt));

       assert(req_pkt_p != NULL);
       assert(req_pkt_p->pkg != NULL);

       int buflen=recv(node->conn_fd,(void *)req_pkt_p,sizeof(struct request),0);

       if(buflen < 0)
       {
           //读取完成
           if(errno== EAGAIN || errno == EINTR){ 
               log_write(CONF.lf,LOG_INFO,"no data:file:%s,line :%d\n",__FILE__,__LINE__);

           }else{
               log_write(CONF.lf,LOG_INFO,"error:file:%s,line :%d\n",__FILE__,__LINE__);                            //error
           }
           free(req_pkt_p->pkg);
           free(req_pkt_p);
           return -1;
       }
       else if(buflen==0)           
       {
           //删除连接节点
           conn_delete(&master_server->conn_root,&node);

           free(req_pkt_p->pkg);
           free(req_pkt_p);
           return 0;
       }
       else if(buflen>0)
       {
          
          log_write(CONF.lf,LOG_INFO,"%s","data comming....\n");
       }
   }
}

