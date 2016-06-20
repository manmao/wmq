#ifndef __MESSAGE_H_
#define __MESSAGE_H_

#define DATA_BUFF_SIZE 1024*5   //pkt包数据的大小


/**
*message
*/
typedef struct message{
	long id;
	char topic[256];
	char from[64];
	char msg_buff[DATA_BUFF_SIZE];
}message_t;



#endif