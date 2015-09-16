#ifndef __FILE_BLOCK_H
#define __FILE_BLOCK_H

//文件分块，文件块的数据结构
struct  file_block
{
	unsigned long block_id;		 //文件块id
	char filename[100];  		//文件块所属的文件名
	unsigned long filesize;    //文件块大小
	unsigned char *block;  	  //文件块内容指针
};

#endif