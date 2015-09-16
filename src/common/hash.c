#include "hash.h"


#define MOD 131

int ELFHash(const char *key)
{
    unsigned long h=0;
    while(*key){
        h=(h<<4)+*key++;
        unsigned long g=h&0XF0000000L;
        if(g)
            h^=g>>24;
        h=h&~g;
    }
    return h%MOD;
}

unsigned int BKDRHash(const char *str)
{
    unsigned int seed=131;
    unsigned int hash=0;
    while(*str)
    {
        hash=hash*seed+(*str++);
    }
    return (hash&0x7fffffff)%MOD;
}

unsigned int SDBHash(const char *str)
{
    unsigned int hash=0;
    while(*str)
    {
        hash=(*str++)+(hash<<6)+(hash<<16)-hash;
    }
    return (hash&0x7fffffff)%MOD;
}

unsigned int RSHash(const char *str)
{
    unsigned int b=378551;
    unsigned int a=63689;
    unsigned int hash=0;
    while(*str)
    {
        hash=hash*a+(*str++);
        a *=b;
    }
    return (hash&0x7FFFFFFF)%MOD;
}

unsigned int JSHash(const char *str)
{
    unsigned int hash=1315423911;
    while(*str)
    {
        hash ^= ( (hash<<5)+(*str++)+(hash>>2) );
    }
    return (hash&0x7FFFFFFF)%MOD;
}

unsigned int PJWHash(const char *str)
{
    unsigned int BitsInUnignedInt=(unsigned int)(sizeof(unsigned int)*8);
    unsigned int ThreeQuarters=(unsigned int)((BitsInUnignedInt*3)/4);
    unsigned int OneEight=(unsigned int)(BitsInUnignedInt/8);
    unsigned int HighBits=(unsigned int)(0xFFFFFFF)<<(BitsInUnignedInt-OneEight);


    unsigned int hash=0;
    unsigned int test=0;
    while(*str)
    {
        hash=(hash << OneEight)+(*str++);
        if((test = hash&HighBits)!=0)
            hash=((hash ^(test >> ThreeQuarters)) & (~HighBits));
    }
    return (hash&0x7FFFFFFF)%MOD;
}

unsigned int DJBhash(const char *str)
{
    unsigned int hash=5381;
    while(*str)
    {
        hash += (hash<<5)+(*str++);
    }
    return (hash&0X7FFFFFFF)%MOD;
}


unsigned int APHash(const char *str)
{
    unsigned int hash=0;
    int i;
    for(i=0;*str;i++)
    {
        if((i&1)==0)
        {
            hash ^= ((hash<<7)^(*str++)^(hash>>3));
        }
        else
            hash ^= (~((hash<<11)^(*str++)^(hash>>5)));
    }

    return (hash & 0X7FFFFFFF)%MOD;
}

