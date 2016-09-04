#ifndef __SERVER_DISPATCH_
#define __SERVER_DISPATCH_

#include "msg_queue.h"
#include "socket_pkg.h"

extern void handle_socket_pkg(mq_t *mq,socket_pkg_t *pkg);


#endif