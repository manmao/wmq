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
#include "util.h"

#include "log.h"

/* external variables */

/*
*  Opens the server log file
*  to be called once at the beginning
*/
void printfloat(FILE *lf,const float flt);
void printstr(FILE *lf,const char *ptr);
void printint(FILE *lf,const int dec);
void printch(FILE *lf,const char ch);

FILE *log_file_open(char logFileName[])
{

    FILE *lf;  //全局文件变量

    int ret=access(logFileName, F_OK); //mode填F_OK
    if(ret == -1) //文件不存在
    {
        printf("%s\n",logFileName);
        int fd=open(logFileName,O_CREAT|O_EXCL|O_RDWR,0644);
        if(fd < 0)
            errExit("CreateLogFile Fail,File:%s , Line:%d\n",__FILE__,__LINE__);
        lf = fdopen(fd,"w+");
    }
    else
    {
       lf = fopen (logFileName, "a");
    }


    if (lf == NULL){
         errExit("OpenLogFile Fail,File:%s , Line:%d\n",__FILE__,__LINE__);
        return NULL;
    }

    char time_str[50];
    get_nowt(time_str);

    fprintf(lf, "\t\n============Server started on================\n%s\n", time_str);
    fflush(lf); /* otherwise we have mysterious problems with fork() !! */

    printf("Opened Log File.\n");

    return lf;
}


/* Closes the log file
 */
int log_file_close(FILE * lf)
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
int log_write(FILE * lf,LOG_TYPE eventType,char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char time_str[100];
    get_nowt(time_str);



    switch (eventType)
    {
    case LOG_ERROR:
    	fprintf(lf,"[%s] [%s] \n",time_str,"ERROR");
		  break;
    case LOG_WARN:
        fprintf(lf,"[%s] [%s] \n",time_str,"WARN");
        break;
    case LOG_INFO:
        fprintf(lf,"[%s] [%s] \n",time_str,"INFO");
		  break;
    default:
        printf ("Unknown event to log! Programming error!\n");
    }

    while(*format)
    {
        if(*format != '%')
        {
            putchar(*format);
            fprintf(lf,"%c",*format);
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

    fprintf(lf,"\n");
    fflush(lf);
    va_end(ap);

    return 0;
}


/**********************************************************
***********************************************************/
void printch(FILE * lf,const char ch)
{
    printf("%c",ch);
    fprintf(lf,"%c",ch);
}

/**********************************************************
***********************************************************/
void printint(FILE * lf,const int dec)
{
    printf("%d",dec);
    fprintf(lf,"%d",dec);
}
/**********************************************************

***********************************************************/
void printstr(FILE * lf,const char *ptr)
{
   printf("%s",ptr);
   fprintf(lf,"%s",ptr);
}

/**********************************************************
***********************************************************/
void printfloat(FILE * lf,const float flt)
{
   printf("%0.3lf",flt);
    fprintf(lf,"%0.3lf",flt);
}





