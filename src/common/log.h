/*
log.h
This file is part of pserv
http://pserv.sourceforge.net

Copyright (c) 2001-2002-2003 Riccardo Mottola. All rights reserved.
mail: rmottola@users.sourceforge.net

This file is free software, released under GPL. Please read acclosed license
*/

#ifndef _LOG_H_
#define _LOG_H_

/* logging event type definitions */

#include <stdio.h>


/*
#define ERROR 1
#define WARN  2
#define INFO  3*/

typedef enum{
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO
}LOG_TYPE;

/* log.c */
FILE * log_file_open(char logFileName[]);
int log_file_close(FILE * lf);
int log_write(FILE * lf,LOG_TYPE eventType,char *format, ...);



#endif