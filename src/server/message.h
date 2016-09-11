#ifndef __MESSAGE_H_
#define __MESSAGE_H_

#include <stdbool.h>  //C99 标准包含的头文件 包括bool
#include <stdint.h>

/*typedef signed char int8_t
typedef unsigned char uint8_t
typedef signed int int16_t
typedef unsigned int uint16_t
typedef signed long int int32_t
typedef unsigned long int uint32_t
typedef signed long long int int64_t
typedef unsigned long long int uint64_t*/

#define DATA_BUFF_SIZE 1024*5   //pkt包数据的大小


/**
*message
*/
typedef struct message{
	uint64_t id;
	int8_t topic[256];
	int8_t from[64];
	int8_t msg_buff[DATA_BUFF_SIZE];
}message_t;



#endif