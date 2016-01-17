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

#include "config.h"

int get_config_string(char *pInFileName,char *pInSectionName,char *pInKeyName,char *pOutKeyValue)
{
    FILE *fpConfig;
    char szBuffer[150];
    char *pStr1,*pStr2;
    int iRetCode = 0;

    /*test*/
    /*
    printf("pInFileName: %s !\n",pInFileName);
    printf("pInSectionName: %s !\n",pInSectionName);
    printf("pInKeyName: %s !\n",pInKeyName);
    */

    memset(szBuffer,0,sizeof(szBuffer));
    if( NULL==( fpConfig=fopen(pInFileName,"r") ) )
        return FILENAME_NOTEXIST;

    while( !feof(fpConfig) )
    {
        if( NULL==fgets(szBuffer,150,fpConfig) )
            break;
        pStr1 = szBuffer ;
        while( (' '==*pStr1) || ('\t'==*pStr1) )
            pStr1++;
        if( '['==*pStr1 )
        {
            pStr1++;
            while( (' '==*pStr1) || ('\t'==*pStr1) )
                pStr1++;
            pStr2 = pStr1;
            while( (']'!=*pStr1) && ('\0'!=*pStr1) )
                pStr1++;
            if( '\0'==*pStr1)
                continue;
            while( ' '==*(pStr1-1) )
                pStr1--;
            *pStr1 = '\0';

            iRetCode = compare_string(pStr2,pInSectionName);
            if( !iRetCode )/*检查节名*/
            {
                iRetCode = get_key_value(fpConfig,pInKeyName,pOutKeyValue);
                fclose(fpConfig);
                return iRetCode;
            }
        }
    }

    fclose(fpConfig);
    return SECTIONNAME_NOTEXIST;

}

/*区分大小写*/
int compare_string(char *pInStr1,char *pInStr2)
{
    if( strlen(pInStr1)!=strlen(pInStr2) )
    {
        return STRING_LENNOTEQUAL;
    }

    /*while( toupper(*pInStr1)==toupper(*pInStr2) )*//*#include <ctype.h>*/
    while( *pInStr1==*pInStr2 )
    {
        if( '\0'==*pInStr1 )
            break;
        pInStr1++;
        pInStr2++;
    }

    if( '\0'==*pInStr1 )
        return STRING_EQUAL;

    return STRING_NOTEQUAL;

}

int get_key_value(FILE *fpConfig,char *pInKeyName,char *pOutKeyValue)
{
    char szBuffer[150];
    char *pStr1,*pStr2,*pStr3;
    unsigned int uiLen;
    int iRetCode = 0;

    memset(szBuffer,0,sizeof(szBuffer));
    while( !feof(fpConfig) )
    {
        if( NULL==fgets(szBuffer,150,fpConfig) )
            break;
        pStr1 = szBuffer;
        while( (' '==*pStr1) || ('\t'==*pStr1) )
            pStr1++;
        if( '#'==*pStr1 )
            continue;
        if( ('/'==*pStr1)&&('/'==*(pStr1+1)) )
            continue;
        if( ('\0'==*pStr1)||(0x0d==*pStr1)||(0x0a==*pStr1) )
            continue;
        if( '['==*pStr1 )
        {
            pStr2 = pStr1;
            while( (']'!=*pStr1)&&('\0'!=*pStr1) )
                pStr1++;
            if( ']'==*pStr1 )
                break;
            pStr1 = pStr2;
        }
        pStr2 = pStr1;
        while( ('='!=*pStr1)&&('\0'!=*pStr1) )
            pStr1++;
        if( '\0'==*pStr1 )
            continue;
        pStr3 = pStr1+1;
        if( pStr2==pStr1 )
            continue;
        *pStr1 = '\0';
        pStr1--;
        while( (' '==*pStr1)||('\t'==*pStr1) )
        {
            *pStr1 = '\0';
            pStr1--;
        }

        iRetCode = compare_string(pStr2,pInKeyName);
        if( !iRetCode )/*检查键名*/
        {
            pStr1 = pStr3;
            while( (' '==*pStr1)||('\t'==*pStr1) )
                pStr1++;
            pStr3 = pStr1;
            while( ('\0'!=*pStr1)&&(0x0d!=*pStr1)&&(0x0a!=*pStr1) )
            {
                if( ('/'==*pStr1)&&('/'==*(pStr1+1)) )
                    break;
                pStr1++;
            }
            *pStr1 = '\0';
            uiLen = strlen(pStr3);
            memcpy(pOutKeyValue,pStr3,uiLen);
            *(pOutKeyValue+uiLen) = '\0';
            return SUCCESS;
        }
    }

    return KEYNAME_NOTEXIST;
}

int get_config_int(char *pInFileName,char *pInSectionName,char *pInKeyName,int *pOutKeyValue)
{
    int iRetCode = 0;
    char szKeyValue[16],*pStr;

    memset(szKeyValue,0,sizeof(szKeyValue));
    iRetCode = get_config_string(pInFileName,pInSectionName,pInKeyName,szKeyValue);
    if( iRetCode )
        return iRetCode;
    pStr    = szKeyValue;
    while( (' '==*pStr)||('\t'==*pStr))
        pStr++;
    if( ('0'==*pStr)&&( ('x'==*(pStr+1))||('X'==*(pStr+1)) ) )
        sscanf(pStr+2,"%x",pOutKeyValue);
    else
        sscanf(pStr,"%d",pOutKeyValue);

    return SUCCESS;

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