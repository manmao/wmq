#if 0
#include <stdio.h>
#include "LzmaLib.h"

int main(int argc, char* argv[])
{
    FILE* pFile = fopen(argv[1], "rb");

    if (pFile == NULL)
    {
        printf("Error to Open the file!\n");
        return  - 1;
    }

    /*文件大小的求法*/
    fseek(pFile, 0, SEEK_END);
    size_t srcLen = ftell(pFile);
    //srcLen为文件的字节数
    rewind(pFile);//重置文件指针
    size_t destLen = srcLen * 2;

    unsigned char* psrcRead = new unsigned char[srcLen]; //原始文件数据
    unsigned char* pLzma = new unsigned char[destLen]; //存放压缩数据
    unsigned char* pDecomress = new unsigned char[srcLen]; //存放解压缩数据

/******************************压缩数据*********************************/
    ///读取文件数据
    fread(psrcRead, sizeof(char), srcLen, pFile);

    ///定义prop参数
    unsigned char prop[5] =
    {
        0
    };
    size_t sizeProp = 5;

    ///压缩文件数据
    if (SZ_OK != LzmaCompress(pLzma, &destLen, psrcRead, srcLen, prop,
        &sizeProp, 9, (1 << 24), 3, 0, 2, 32, 2))
    {
        //出错了
        printf("压缩时出错\n");
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;
        fclose(pFile);
        return  - 1;
    }
    ///打开文件

    FILE* pCompressFile = fopen("compress.dat", "wb");
    //写入压缩后的数据
    if (pCompressFile == NULL)
    {
        printf("创建文件出错!\n");
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;
        fclose(pFile);
        return  - 1;
    }

    ///写入压缩后的数据到文件
    fwrite(pLzma, sizeof(char), destLen, pCompressFile);
    fclose(pCompressFile);

/***********************解压数据***********************************/
    FILE* pDecompressFile = fopen("decompress.dat", "wb");
    //写入解压缩数据
    if (pDecompressFile == NULL)
    {
        printf("写入数据出错！\n");
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;
        fclose(pFile);
        return  - 1;
    }

    //注意：解压缩时props参数要使用压缩时生成的outProps，这样才能正常解压缩
    if (SZ_OK != LzmaUncompress(pDecomress, &srcLen, pLzma, &destLen, prop, 5))
    {
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;

        fclose(pDecompressFile);
        fclose(pFile);
        return  - 1;
    }
    //将解压出来的数据写入到文件
    fwrite(pDecomress, sizeof(char), srcLen, pDecompressFile);

    delete [] psrcRead;
    delete [] pDecomress;
    delete [] pLzma;

    fclose(pDecompressFile);
    fclose(pFile);

    return 0;
}

#end
