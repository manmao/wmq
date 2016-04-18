#include "handler.h"


void master_handle_request(void *arg){

   struct conn_node *node=(struct conn_node *)arg;

   struct request *req_pkt_p=NULL;  

   while(1)
   {
       req_pkt_p =(struct request*)malloc(sizeof(struct request));
       req_pkt_p->pkg=(struct sock_pkt *)malloc(sizeof(struct sock_pkt));
       //......

       assert(req_pkt_p != NULL);
       assert(req_pkt_p->pkg != NULL);
   
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
       else if(buflen==0)           
       {
            deletefd(node->epoll_fd,node->conn_fd);

            //struct conn_node node;
            //node.conn_fd=event_fd;
            //conn_delete(&server->conn_root,&node);

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

