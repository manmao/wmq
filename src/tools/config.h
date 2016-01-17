#ifndef __GET_CONFVALUE_H
#define __GET_CONFVALUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#define  SUCCESS           0x00 /*成功*/
#define  FAILURE           0x01 /*失败*/

#define  FILENAME_NOTEXIST      0x02 /*配置文件名不存在*/
#define  SECTIONNAME_NOTEXIST    0x03 /*节名不存在*/
#define  KEYNAME_NOTEXIST      0x04 /*键名不存在*/
#define  STRING_LENNOTEQUAL     0x05 /*两个字符串长度不同*/
#define  STRING_NOTEQUAL       0x06 /*两个字符串内容不相同*/
#define  STRING_EQUAL        0x00 /*两个字符串内容相同*/


int compare_string(char *pInStr1,char *pInStr2);
int get_key_value(FILE *fpConfig,char *pInKeyName,char *pOutKeyValue);
int get_config_int(char *pInFileName,char *pInSectionName,char *pInKeyName,int *pOutKeyValue);
int get_config_string(char *pInFileName,char *pInSectionName,char *pInKeyName,char *pOutKeyValue);

//从文件中读取ip地址
extern int get_ip(char *path,char ip[][20],int *ipnum);

#ifdef __cplusplus
}
#endif

#endif