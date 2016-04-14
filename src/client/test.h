#ifndef __TEST_H_
#define __TEST_H_

#include <string.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#include "server_util.h"
#include "log.h"
#include "timer_list.h"
#include "config.h"
#include "rbtree.h"
#include "threadpool.h"
#include "util.h"
#include "upload_file.h"

#include "md5.h"


void testgetvalue()
{
	char KeyVal[100];
	get_config_string("config/wfs_config.conf","wfs","name",KeyVal);
	printf("%s\n",KeyVal);
}

static void handleterm(int sig)
{
    return;
}


void *handle(void *arg)
{
	printf("num is %d\n",*(int *)arg);
}

void testthreadpool()
{
	int i;
	struct threadpool *t=threadpool_init(10,10000);
	for(i=0;i<50;i++)
	{
		threadpool_add_job(t,&handle,&i);
		usleep(30);
		//if(i%20==0)
		//sleep(5);
	}

	//sleep(5);
	//threadpool_destroy(t);
}





/**************************
	测试MD5算法
****************************/
void testmd5()
{
	unsigned char md5_checksum[16];  //md5校验值
	unsigned char md5_checksum1[16]; //md5校验值

	char buff[]="adlgkasldg;asdg;asdfljkasflkjadsfoioasdpfjiapsdfjioadlgkasldg;asdg;asdfljkasflkjadsfoioasdpfjiapsdfjioadlgkasldg;asdg;asdfljkasflkjadsfoioasdpfjiapsdfjo";
   char buff1[]="adlgkasldg;asdg;asdfljkasflkjadsfoioasdpfjiapsdfjioadlgkasldg;asdg;asdfljkasflkjadsfoioasdpfjiapsdfjioadlgkasldg;asdg;asdfljkasflkjadsfoioasdpfjiapsdfjo";

	int r=MD5Buffer(buff,strlen(buff)+1,md5_checksum);
	//assert(r==0);
	r=MD5Buffer(buff1,strlen(buff1)+1,md5_checksum1);
	//assert(r==0);

    int ret=is_equal(md5_checksum,md5_checksum1);
	printf("%d\n",ret);
}

static void gname_frpath(const char *path,char *name)
{
	int index=-1;
	int i=0;

	while(path[i] != '\0')
	{
		if(path[i] == '/')
		{
			index=i;
		}
		i++;
	}

	if(index == -1)
	{
		strcpy(name,path);
		return;
	}

	int j=0;
	for(i=index+1;path[i]!='\0';i++,j++)
	{
		name[j]=path[i];
	}
}

void testhash()
{
	//printf("%d\n",DJBhash("sda;l;as")%4);
	char name[10];
	gname_frpath("h.aasdj",name);
	puts(name);
}

void testgetip()
{
	char ip[4][20];
	int num=0;
	get_ip("config/master",ip,&num);
	int i;
	printf("%d\n",num);

	for(i=0;i<num;i++)
	{
		printf("ip%d : %s\n",i,ip[i]);
	}
}




#endif