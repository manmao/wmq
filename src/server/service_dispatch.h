#ifndef __SERVER_DISPATCH_
#define __SERVER_DISPATCH_

#include "server_util.h"
#include "msg_queue.h"
#include "socket_pkg.h"

extern void handle_socket_pkg(server_t *master_server,socket_pkg_t *pkg);

#endif