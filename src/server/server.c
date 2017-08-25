#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "socket_pkg.h"
#include "config.h"
#include "log.h"

#include "server.h"
#include "service_dispatch.h"
#include "mq_receiver.h"
#include "msg_queue.h"


server_t *master_server;

void handle_request(void *arg);

static
void handle_sig(int sig)
{
    printf("catch sig %d\n",sig);
    exit(-1);
}

static 
int handle_unknown(int event_fd)
{
  return 0;
}

static
int on_accept(int client_conn_fd,struct sockaddr clientaddr)
{
  //往红黑树中插入节点
  struct conn_type *type=(struct conn_type *)malloc(sizeof(struct conn_type));
  type->node=(struct conn_node *)malloc(sizeof(struct conn_node));
  type->node->conn_fd = client_conn_fd;
  type->node->epoll_fd = master_server->efd;
  type->node->clientaddr = clientaddr;
  conn_insert(&(master_server->conn_root),type);
  return 0;
}

static
int on_readable(int readable_fd)
{
    struct conn_type *type=NULL;

    struct conn_node node;
    node.conn_fd=readable_fd;
    type=conn_search(&(master_server->conn_root),&node);

    //将数据包加入任务队列
    //放入线程池
    threadpool_add_job(master_server->tpool,(void *)&handle_request,type->node);
    return 0;
}


//子进程中启动监听线程,用于不断的从MQ中读取数据
static
int handle_listenmq()
{
   //开启线程组，监听对应的消息队列,master_server->mq[i]每个消息队列建立一个线程监听
   pthread_t receiver_tid[20];
   int i;
   for(i = 0; i < master_server->queues; i++){
      master_server->mq[i]->ht=master_server->ht;
      pthread_create(&receiver_tid[i],NULL,(void *)&msg_queue_receiver,master_server->mq[i]);
   }

   return 0;
}

void handle_request(void *arg){

   struct conn_node *node=(struct conn_node *)arg;
   socket_pkg_t *socket_pkt_ptr=NULL;
   pkg_header_t *header=NULL;  
   while(1)
   {
       header=create_pkg_header_instance(); //创建header实例
       assert(header != NULL);
       int buflen=recv(node->conn_fd,(void *)header,sizeof(header),0);

       if(buflen < 0)
       {
           //读取完成
           if(errno== EAGAIN || errno == EINTR){ 
               log_write(CONF.lf,LOG_INFO,"no data:file:%s,line :%d\n",__FILE__,__LINE__);
           }else{
               log_write(CONF.lf,LOG_INFO,"error:file:%s,line :%d\n",__FILE__,__LINE__);                            //error
           }
           free(header);
           header=NULL;
           return ;
       }else if(buflen==0){

          //删除连接节点
          conn_delete(&master_server->conn_root,node);
          free(header);
          header=NULL;
          return ;

       }else if(buflen>0 ){

          socket_pkt_ptr=create_socket_pkg_instance();
          socket_pkt_ptr=add_header(socket_pkt_ptr,header);
          socket_pkt_ptr->fd=node->conn_fd;

          int res=recv(node->conn_fd,(void *)socket_pkt_ptr->msg,socket_pkt_ptr->data_len,0);
          if(res<0)
            log_write(CONF.lf,LOG_ERROR,"#####receive data body fail!!!###########");
          handle_socket_pkg(master_server,socket_pkt_ptr);
          log_write(CONF.lf,LOG_INFO,"data len:%d ,data checksum:%d\n",socket_pkt_ptr->data_len, socket_pkt_ptr->checksum);
       
       }

       free(header);
       header=NULL;


   }
}

int server_init(int argc,char *argv[])
{
    //挂接服务器事件处理函数
    struct server_handler *handler=(struct server_handler *)malloc(sizeof(struct server_handler));
    //如果没有相关接口实现的，一定要赋值为空值
    handler->handle_readable=&on_readable;
    handler->handle_accept=&on_accept;
    handler->handle_unknown=&handle_unknown;
    handler->handle_sig=&handle_sig;
    handler->handle_listenmq=&handle_listenmq;

    master_server=(struct server *)malloc(sizeof(struct server));
    init_server(master_server,NET_CONF.ip,NET_CONF.port,handler);
    
    //开始监听
    start_listen(master_server,8,10000); //启动服务器监听子进程

    return 0;
}
