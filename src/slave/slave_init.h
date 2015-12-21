#ifndef __SLAVE_INIT_H_
#define __SLAVE_INIT_H_

#include <sys/epoll.h>

#include "server_sockopt.h"

//slave 服务器初始化
int slave_server_init(int argnum,char *argv[]);

void on_slave_handle(struct sock_server *server,struct epoll_event events);

#endif