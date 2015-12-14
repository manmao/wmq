#ifndef __GET_CONFVALUE_H
#define __GET_CONFVALUE_H

#ifdef __cplusplus
extern "C"
{
#endif 

extern char * a_trim(char * szOutput, const char * szInput);

//读取配置文件参数
extern int get_filestr(char *profile,char *Appname,char *KeyName,char *KeyVal);

//从文件中读取ip地址
extern int get_ip(char *path,char ip[][20],int *ipnum);

#ifdef __cplusplus
}
#endif 

#endif