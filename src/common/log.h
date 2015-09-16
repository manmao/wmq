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

#define ERROR 1
#define WARN  2
#define INFO  3

#define LOG_DEFAULT_PATH "/home/linux/log.txt"      //暂时日志文件路径

/* log.c */
FILE * logFileOpen(char logFileName[]); 
int logFileClose(FILE * lf);
int logWriter(FILE * lf,int eventType,char *format, ...);

#endif