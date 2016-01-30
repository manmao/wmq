#ifndef __HASH__H_
#define __HASH__H_

#ifdef __cplusplus
	extern "C"{
#endif

extern int ELFHash(const char *key);
extern unsigned int BKDRHash(const char *str);
extern unsigned int SDBHash(const char *str);
extern unsigned int RSHash(const char *str);
extern unsigned int JSHash(const char *str);
extern unsigned int PJWHash(const char *str);
extern unsigned int DJBhash(const char *str);
extern unsigned int APHash(const char *str);

#ifdef __cplusplus
	}
#endif

#endif