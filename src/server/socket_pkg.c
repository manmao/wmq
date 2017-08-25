#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "socket_pkg.h"

/**
 * 创建一个实例
 * @return [description]
 */
struct socket_pkg* create_socket_pkg_instance(){
	socket_pkg_t *socket_pkt_ptr=NULL;  
	socket_pkt_ptr =(struct socket_pkg*)malloc(sizeof(struct socket_pkg));
	assert(socket_pkt_ptr != NULL);
    return socket_pkt_ptr;
}


struct pkg_header* create_pkg_header_instance(){
	pkg_header_t *header=NULL;  
	header =(struct pkg_header*)malloc(sizeof(struct pkg_header));
	assert(header != NULL);
    return header;
}

struct socket_pkg* add_header(struct socket_pkg* pkg,struct pkg_header*header){
	
	pkg->version=header->version;
	pkg->code=header->code;
	pkg->fd=header->fd;
	memcpy(pkg->topic,header->topic,sizeof(header->topic)/sizeof(header->topic[0]));
	pkg->cmd=header->cmd;
	pkg->data_len=header->data_len;
	pkg->checksum=header->checksum;
	memcpy(pkg->from,header->from,sizeof(header->from)/sizeof(header->from[0]));
	pkg->msg =(uint8_t*)malloc(sizeof(uint8_t)*pkg->data_len); //开辟msg空间

	return pkg;
}

/**
 * 销毁一个实例
 */
void destroy_socket_pkg_instance(socket_pkg_t *socket_pkt_ptr){
	if(socket_pkt_ptr !=NULL ){
		if(socket_pkt_ptr->msg != NULL)
			free(socket_pkt_ptr->msg);
     	free(socket_pkt_ptr);
     	socket_pkt_ptr=NULL;
	}
}
