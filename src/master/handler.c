#include "handler.h"

void *master_handle_request(void *arg){

   struct conn_node *node=(struct conn_node *)arg;

   struct request *req_pkt_p=NULL;  //网络请求数据包

   while(1)
   {
       //开辟空间
       req_pkt_p =(struct request*)malloc(sizeof(struct request));
       req_pkt_p->pkg=(struct sock_pkt *)malloc(sizeof(struct sock_pkt));
       //......

       assert(req_pkt_p != NULL);
       assert(req_pkt_p->pkg != NULL);
       //接收客户端发送过来的数据
       int buflen=recv(node->conn_fd,(void *)req_pkt_p,sizeof(struct request),0);

       if(buflen < 0)
       {
           if(errno== EAGAIN || errno == EINTR){ //即当buflen<0且errno=EAGAIN时，表示没有数据了。(读/写都是这样)
               log_write(CONF.lf,LOG_INFO,"no data:file:%s,line :%d\n",__FILE__,__LINE__);

           }else{
               log_write(CONF.lf,LOG_INFO,"error:file:%s,line :%d\n",__FILE__,__LINE__);                            //error
           }
           free(req_pkt_p->pkg);
           free(req_pkt_p);
           return -1;
       }
       else if(buflen==0)              //客户端断开连接
       {
            /**将文件描述符从epoll队列中移除**/
            deletefd(node->epoll_fd,node->conn_fd);

            /*******删除连接队列中的点*******/
            //struct conn_node node;
            //node.conn_fd=event_fd;
            //conn_delete(&server->conn_root,&node);

           free(req_pkt_p->pkg);
           free(req_pkt_p);
           return 0;
       }
       else if(buflen>0) //客户端发送数据过来了
       {
            printf("data comming....\n");
            //处理req_pkt_p
       }
   }
}

