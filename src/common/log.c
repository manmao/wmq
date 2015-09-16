/*
log.c
This file is part of pserv
http://pserv.sourceforge.net

Copyright (c) 2001-2002 Riccardo Mottola. All rights reserved.
mail: rmottola@users.sourceforge.net

This file is free software, released under GPL. Please read acclosed license
*/

#include <stdio.h> 
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

#include "error_hdr.h"
#include "log.h"

/* external variables */



/* Opens the server log file
 * to be called once at the beginning
 */

void printfloat(FILE *lf,const float flt);
void printstr(FILE *lf,const char *ptr);
void printint(FILE *lf,const int dec);
void printch(FILE *lf,const char ch);


FILE *logFileOpen(char logFileName[])
{
    char timeStr[256];
    struct tm *timeStruct;
    time_t timeTemp;
    FILE *lf;  //全局文件变量

    time(&timeTemp);
    timeStruct = (struct tm *) localtime(&timeTemp);
    strftime(timeStr, 256, "[ %d-%b-%Y %H:%M:%S %Z ]", timeStruct);

    int ret=access(logFileName, F_OK); //mode填F_OK试试。
    if(ret == -1) //文件不存在
    {
        printf("%s\n",logFileName);
        int fd=open(logFileName,O_CREAT|O_WRONLY,0644);
        if(fd < 0)
        {
            errExit("logFileName\n");
            printf("创建文件失败");
            exit(-1);
        }
        close(fd);
    }

    lf = fopen (logFileName, "a");

    if (lf == NULL)
        return NULL;
    fprintf(lf, "\n===Server started on===\n%s\n", timeStr);
    fflush(lf); /* otherwise we have mysterious problems with fork() !! */ 

    printf("Opened Log File.\n");

    return lf;
}

/* Closes the log file
 */
int logFileClose(FILE * lf)
{
    fclose(lf);
   	printf("Closed Log File.\n");
    return 0;
}


/* 
    Writes an event in the log.
 * the event kind is given by eventType
 */

 /***
  *日志系统写入
  *函数名 logWriter
  *函数参数  
  *      @param eventType  日志类型 
            #define ERROR  错误
            #define WARN   警告
            #define INFO   信息

  *      @param *format    格式化字符串 "%s %d %f"
  *      @param ...        参数
  *函数返回 int
  *日期 2015-03-26
  *
  ***/
int logWriter(FILE * lf,int eventType,char *format, ...) 
{
    va_list ap;
    char timeStr[256];
    struct tm *timeStruct;
    time_t timeTemp;

    va_start(ap, format);

    time(&timeTemp);
    timeStruct = (struct tm *)localtime(&timeTemp); //获取本地时间

    strftime(timeStr, 256, "%d-%b-%Y  %H:%M:%S %Z ", timeStruct);

    switch (eventType)
    {
    case ERROR:
    	fprintf(lf,"[%s] [%s] ",timeStr,"ERROR");
		break;
    case WARN:
        fprintf(lf,"[%s] [%s] ",timeStr,"WARN");
        break;
    case INFO:
        fprintf(lf,"[%s] [%s] ",timeStr,"INFO");
		break;
    default:
        printf ("Unknown event to log! Programming error!\n");
    }
    
    while(*format)
    {
        if(*format != '%')
        {
            putchar(*format);
            format++;
        }
        else
        {
            format++;
            switch(*format)
            {
                case 'c':
                 {
                    char valch = va_arg(ap,int);
                     printch(lf,valch);
                     format++;
                    break;
                }
                case 'd':
                {
                    int valint = va_arg(ap,int);
                    printint(lf,valint);
                    format++;
                    break;
                }
                 case 's':
                {
                    char *valstr = va_arg(ap,char *);
                    printstr(lf,valstr);
                    format++;
                    break;
                }
                case 'f':
                {
                     float valflt = va_arg(ap,double);
                     printfloat(lf,valflt);
                    format++;
                    break;
                 }
                default:
                {
                     printch(lf,*format);
                     format++;
                }
            }
        }
    }

    fflush(lf);
    va_end(ap);
    
    return 0;
}


/**********************************************************
***********************************************************/
void printch(FILE * lf,const char ch)
{
    fprintf(lf,"%c",ch);
}

/**********************************************************
***********************************************************/
void printint(FILE * lf,const int dec)
{
    fprintf(lf,"%d",dec);
}
/**********************************************************

***********************************************************/
void printstr(FILE * lf,const char *ptr)
{
   fprintf(lf,"%s",ptr);
}

/**********************************************************
***********************************************************/
void printfloat(FILE * lf,const float flt)
{
    fprintf(lf,"%0.3lf",flt);
}





