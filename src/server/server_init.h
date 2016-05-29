#ifndef __MASTER_server_t_INIT_H__
#define __MASTER_server_t_INIT_H__


#include <sys/epoll.h>
#include "server_util.h"

/**
 * [master_server初始化]
 * @param  argc [参数个数]
 * @param  argv [参数]
 * @return      [状态值]
 */
extern int server_init(int argc,char *argv[]);


/**
 * 获取客户端请求包
 * @param arg struct conn_node连接节点
 */
extern void handle_request(void *arg);


#endif