#ifndef __META_DATA_H_
#define __META_DATA_H_

#include <stdint.h>
#include <netinet/in.h>
#include <sys/socket.h>

//文件定位数据		///
struct loc_file{
	struct sockaddr_in  addr;  		  /* 文件块所在的网络地址 		*/
	uint16_t            fb_name;      /* 文件块的名字,通过数字表示 */
	uint8_t             fb_size;   	  /* 文件大小 				   */
	char 				*path;		  /* 文件的存储路径(完整路径，包括文件名) */
};

//元数据设计		///
typedef struct _metadata
{
	uint8_t   		    f_id;  	 	   /*  文件编号         */
	uint8_t  		   *fsversion; 	   /*  文件系统版本     */
	uint8_t  		   *f_name;    	   /*  文件名字         */
	uint16_t  		   f_size;    	   /*  文件大小         */
	uint8_t   		   f_cttime;  	   /*  文件创建日期     */
	uint16_t 		   checksum;       /*  数据校验和       */
	struct loc_file    *loc;       	   /*  元素文件块的位置 */
}META_DATA_NODE;

#endif