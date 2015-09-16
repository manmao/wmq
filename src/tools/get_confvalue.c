#define _GNU_SOURCE
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "error_hdr.h"

#define KEYVALLEN 100

/*   删除左边的空格   */
char * l_trim(char * szOutput, const char *szInput)
{
 assert(szInput != NULL);
 assert(szOutput != NULL);
 assert(szOutput != szInput);
 for(NULL; *szInput != '\0' && isspace(*szInput); ++szInput){
 	;}
 	return strcpy(szOutput, szInput);
}


/* 删 除 右 边 的 空 格 */
char *r_trim(char *szOutput, const char *szInput)
{
	char *p = NULL;
	assert(szInput != NULL);
	assert(szOutput != NULL);
	assert(szOutput != szInput);
	strcpy(szOutput, szInput);
	for(p = szOutput + strlen(szOutput) - 1;
	 p >= szOutput && isspace(*p);
	  --p){;}
	*(++p) = '\0';
	return szOutput;
}


/*   删除两边的空格   */
char * a_trim(char * szOutput, const char * szInput)
{
	char *p = NULL;
	assert(szInput != NULL);
	assert(szOutput != NULL);
	l_trim(szOutput, szInput);
	for(p = szOutput + strlen(szOutput) - 1;p >= szOutput && isspace(*p); --p){
		;
	}
	*(++p) = '\0';
	return szOutput;
}

/***********************************
*函数名称:get_filestr
*函数参数：
*		@param profile  配置文件的相对路径
*		@param AppName  AppName名字
*		@param KeyName  键名
*		@param KeyVal   键值
*函数返回:
*		@return int 
*
*函数描述:		
*********************************/
int get_filestr(char *profile,char *Appname,char *KeyName,char *KeyVal)
{
	char appname[32],keyname[32];
	char *buf,*c;
	char buf_i[KEYVALLEN],buf_o[KEYVALLEN];
	FILE *fp;
	int found=0;	/* 1 AppName 2 KeyName */
	if((fp=fopen( profile,"r" ))==NULL )
	{
		printf("open file [%s] error %s",profile,strerror(errno));
		return -1;
	}

	fseek(fp,0,SEEK_SET);

	memset(appname,0,sizeof(appname));
	sprintf(appname,"[%s]",Appname);

	//当没有到文件末尾 并且读取的一行数据不为空
	while(!feof(fp) && fgets(buf_i,KEYVALLEN,fp)!=NULL)
	{
		l_trim(buf_o,buf_i);
		if(strlen(buf_o) <= 0)
			continue;
		buf=NULL;
		buf=buf_o;

		if(found == 0)   //   匹配[appname]
		{
			if(buf[0] != '[')
				continue;
			else if (strncmp(buf,appname,strlen(appname))==0)
			{
				/* code */
				found=1;
				continue;
			}
			
		}
		else if(found == 1)  // 匹配 found == 1
		{
			if(buf[0] == '#')
				continue;
			else if(buf[0] == '[')
				break;
			else
			{
				if((c=(char *)strchr(buf,'=')) == NULL )
					continue;
				memset(keyname,0,sizeof(keyname));
				sscanf(++c,"%[^\n]",KeyVal);
				char *KeyVal_o = (char *)malloc(strlen(KeyVal) + 1);
				if(KeyVal_o != NULL)
				{
					memset(KeyVal_o, 0, sizeof(KeyVal_o));
					a_trim(KeyVal_o, KeyVal);
					if(KeyVal_o && strlen(KeyVal_o) > 0)
					{
						strcpy(KeyVal, KeyVal_o);
						free(KeyVal_o);
						KeyVal_o = NULL;
						found = 2;
						break;
					}
					else
					{
						continue;
					}

				}
			}
		}

	}

	fclose( fp );

	if( found == 2 )
		return 0;
	else
		return -1;
}



/**********************************
	从配置文件中读取ip地址
函数参数：
	@param path{char *}  配置文件的路径

	@param ip{char *}   获取到的ip地址 

	@param ipnum{int *} 获取到的ip数量

	@return 
		int  成功返回0，失败返回-1
*********************************/
int 
get_ip(char *path,char ip[][20],int *ipnum)
{
	FILE *fp;
	char * line = NULL;
	if((fp=fopen(path,"r"))==NULL)
	{
		errExit("open configure file fail\n");
		return -1;
	}

	*ipnum=0;
	size_t len=0;
	ssize_t read;
	//从文件中读取一行出来，复制给line
	while((read = getline(&line, &len, fp)) != -1)
	{
		strcpy(ip[*ipnum],line);
		(*ipnum)++;
	}



	if(line)
		free(line);
	fclose(fp);
	return *ipnum;

}