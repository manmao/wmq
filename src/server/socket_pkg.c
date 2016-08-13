#include<stdlib.h>

#include "socket_pkg.h"
#include "message.h"

/**
 * 创建一个实例
 * @return [description]
 */
struct socket_pkg* create_socket_pkg_instance(){
	socket_pkg_t *socket_pkt_ptr=NULL;  
	socket_pkt_ptr =(struct socket_pkg*)malloc(sizeof(struct socket_pkg));
    socket_pkt_ptr->msg=(message_t *)malloc(sizeof(message_t ));
    return socket_pkt_ptr;
}


/**
 * 销毁一个实例
 */
void destroy_socket_pkg_instance(socket_pkg_t *socket_pkt_ptr){
	if(socket_pkt_ptr !=NULL && socket_pkt_ptr->msg !=NULL)
		free(socket_pkt_ptr->msg);
	if(socket_pkt_ptr !=NULL )
     	free(socket_pkt_ptr);

}
