#include<stdlib.h>

#include "socket_pkg.h"

/**
 * 创建一个实例
 * @return [description]
 */
struct socket_pkg* create_socket_pkg_instance(){
	socket_pkg_t *socket_pkt_ptr=NULL;  
	socket_pkt_ptr =(struct socket_pkg*)malloc(sizeof(struct socket_pkg));
	assert(socket_pkt_ptr != NULL)
    return socket_pkt_ptr;
}


/**
 * 销毁一个实例
 */
void destroy_socket_pkg_instance(socket_pkg_t *socket_pkt_ptr){
	if(socket_pkt_ptr !=NULL )
     	free(socket_pkt_ptr);
}
