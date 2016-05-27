#ifndef __SLAVE_INIT_H_
#define __SLAVE_INIT_H_

#include <sys/epoll.h>



/**
 * 服务器初始化
 * @param  argnum  参数个数
 * @param  argv    参数质量
 * @return         装填返回
 */
int slave_server_init(int argnum,char *argv[]);


#endif