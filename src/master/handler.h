#ifndef __MASTER_HANDLER_H
#define __MASTER_HANDLER_H

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "common_define.h"
#include "config.h"
#include "log.h"

#include "server_util.h"


extern void master_handle_request(void *arg);



#endif
