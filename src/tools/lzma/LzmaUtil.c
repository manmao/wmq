/* LzmaUtil.c -- Test application for LZMA compression
2010-09-20 : Igor Pavlov : Public domain */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Alloc.h"
#include "7zFile.h"
#include "7zVersion.h"
#include "LzmaDec.h"
#include "LzmaEnc.h"

const char *kCantReadMessage = "Can not read input file";
const char *kCantWriteMessage = "Can not write output file";
const char *kCantAllocateMessage = "Can not allocate memory";
const char *kDataErrorMessage = "Data error";

static void *SzAlloc(void *p, size_t size) { p = p; return MyAlloc(size); }
static void SzFree(void *p, void *address) { p = p; MyFree(address); }
static ISzAlloc g_Alloc = { SzAlloc, SzFree };

void PrintHelp(char *buffer)
{
  strcat(buffer, "\nLZMA Utility " MY_VERSION_COPYRIGHT_DATE "\n"
      "\nUsage:  lzma <e|d> inputFile outputFile\n"
             "  e: encode file\n"
             "  d: decode file\n");
}

int PrintError(char *buffer, const char *message)
{
  strcat(buffer, "\nError: ");
  strcat(buffer, message);
  strcat(buffer, "\n");
  return 1;
}

int PrintErrorNumber(char *buffer, SRes val)
{
  sprintf(buffer + strlen(buffer), "\nError code: %x\n", (unsigned)val);
  return 1;
}

int PrintUserError(char *buffer)
{
  return PrintError(buffer, "Incorrect command");
}

#define IN_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 16)

static SRes Decode2(CLzmaDec *state, ISeqOutStream *outStream, ISeqInStream *inStream,
    UInt64 unpackSize)
{
  int thereIsSize = (unpackSize != (UInt64)(Int64)-1);
  Byte inBuf[IN_BUF_SIZE];
  Byte outBuf[OUT_BUF_SIZE];
  size_t inPos = 0, inSize = 0, outPos = 0;
  LzmaDec_Init(state);
  for (;;)
  {
    if (inPos == inSize)
    {
      inSize = IN_BUF_SIZE;
      RINOK(inStream->Read(inStream, inBuf, &inSize));
      inPos = 0;
    }
    {
      SRes res;
      SizeT inProcessed = inSize - inPos;
      SizeT outProcessed = OUT_BUF_SIZE - outPos;
      ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
      ELzmaStatus status;
      if (thereIsSize && outProcessed > unpackSize)
      {
        outProcessed = (SizeT)unpackSize;
        finishMode = LZMA_FINISH_END;
      }
      
      res = LzmaDec_DecodeToBuf(state, outBuf + outPos, &outProcessed,
        inBuf + inPos, &inProcessed, finishMode, &status);
      inPos += inProcessed;
      outPos += outProcessed;
      unpackSize -= outProcessed;
      
      if (outStream)
        if (outStream->Write(outStream, outBuf, outPos) != outPos)
          return SZ_ERROR_WRITE;
        
      outPos = 0;
      
      if (res != SZ_OK || (thereIsSize && unpackSize == 0))
        return res;
      
      if (inProcessed == 0 && outProcessed == 0)
      {
        if (thereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK)
          return SZ_ERROR_DATA;
        return res;
      }
    }
  }
}

static SRes Decode(ISeqOutStream *outStream, ISeqInStream *inStream)
{
  UInt64 unpackSize;
  int i;
  SRes res = 0;
  
  CLzmaDec state;

  /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
  unsigned char header[LZMA_PROPS_SIZE + 8];

  /* Read and parse header */

  RINOK(SeqInStream_Read(inStream, header, sizeof(header)));

  unpackSize = 0;
  for (i = 0; i < 8; i++)
    unpackSize += (UInt64)header[LZMA_PROPS_SIZE + i] << (i * 8);

  LzmaDec_Construct(&state);
  RINOK(LzmaDec_Allocate(&state, header, LZMA_PROPS_SIZE, &g_Alloc));
  res = Decode2(&state, outStream, inStream, unpackSize);
  LzmaDec_Free(&state, &g_Alloc);
  return res;
}

static SRes Encode(ISeqOutStream *outStream, ISeqInStream *inStream, UInt64 fileSize, char *rs)
{
  CLzmaEncHandle enc;
  SRes res;
  CLzmaEncProps props;

  rs = rs;

  enc = LzmaEnc_Create(&g_Alloc);
  if (enc == 0)
    return SZ_ERROR_MEM;

  LzmaEncProps_Init(&props);
  res = LzmaEnc_SetProps(enc, &props);

  if (res == SZ_OK)
  {
    Byte header[LZMA_PROPS_SIZE + 8];
    size_t headerSize = LZMA_PROPS_SIZE;
    int i;

    res = LzmaEnc_WriteProperties(enc, header, &headerSize);
    for (i = 0; i < 8; i++)
      header[headerSize++] = (Byte)(fileSize >> (8 * i));
    if (outStream->Write(outStream, header, headerSize) != headerSize)
      res = SZ_ERROR_WRITE;
    else
    {
      if (res == SZ_OK)
        res = LzmaEnc_Encode(enc, outStream, inStream, NULL, &g_Alloc, &g_Alloc);
    }
  }
  LzmaEnc_Destroy(enc, &g_Alloc, &g_Alloc);
  return res;
}

/********************************
*使用命令行压缩，或者解压文件
*通过这个函数可以实现文件的压缩或者文件的解压
*例如：
*
*   char rs[800];
*   EDcode('e',inputfilepath,outputfilepath,rs);  //压缩文件
*   EDcode('d',inputfilepath,outputfilepath,rs);  //解压文件
*   
*函数名称：EDcode
*函数参数：
        @param  mode    //使用模式 e 、d
        @param  *src   //源地址路径
        @param  *dest   //目标路径
        @param  *rs      //提示信息
*函数功能：
*函数返回：
        @return 返回结果 通过 fputs(rs, stdout); 将结果打印到终端
*******************************/
  /*
  typedef struct
  {
    ISeqInStream s;
    CSzFile file;
  } CFileSeqInStream;
  */

  /**
    typedef struct
    {
    SRes (*Read)(void *p, void *buf, size_t *size);
      //** if (input(*size) != 0 && output(*size) == 0) means end_of_stream.
      // (output(*size) < input(*size)) is allowed ****
    }ISeqInStream;
  */
  /*
  typedef struct
  {
   #ifdef USE_WINDOWS_FILE
    HANDLE handle;
    #else
    FILE *file;
    #endif
  } CSzFile;
  */
int EDCode(const char mode,const char *src,const char *dest, char *rs)
{
  CFileSeqInStream inStream;// FILE *file;
  CFileOutStream outStream;
  char c;
  int res;
  int encodeMode;
  Bool useOutFile = False;

  FileSeqInStream_CreateVTable(&inStream);
  File_Construct(&inStream.file);

  FileOutStream_CreateVTable(&outStream);
  File_Construct(&outStream.file);

  c = mode;
  printf("%c\n",c);
  encodeMode = (c == 'e' || c == 'E');
  if (!encodeMode && c != 'd' && c != 'D')
    return PrintUserError(rs);

  {
    size_t t4 = sizeof(UInt32);
    size_t t8 = sizeof(UInt64);
    if (t4 != 4 || t8 != 8)
      return PrintError(rs, "Incorrect UInt32 or UInt64");
  }

//输入文件流
  printf("%s\n",src);
  if (InFile_Open(&inStream.file, src) != 0)
    return PrintError(rs, "Can not open input file");
//输出文件流
    useOutFile = True;
    if (OutFile_Open(&outStream.file, dest) != 0)
      return PrintError(rs, "Can not open output file");

  if (encodeMode)
  {
    UInt64 fileSize;
    File_GetLength(&inStream.file, &fileSize);
    res = Encode(&outStream.s, &inStream.s, fileSize, rs);
  }
  else
  {
    res = Decode(&outStream.s, useOutFile ? &inStream.s : NULL);
  }

  if (useOutFile)
    File_Close(&outStream.file);
  File_Close(&inStream.file);

  if (res != SZ_OK)
  {
    if (res == SZ_ERROR_MEM)
      return PrintError(rs, kCantAllocateMessage);
    else if (res == SZ_ERROR_DATA)
      return PrintError(rs, kDataErrorMessage);
    else if (res == SZ_ERROR_WRITE)
      return PrintError(rs, kCantWriteMessage);
    else if (res == SZ_ERROR_READ)
      return PrintError(rs, kCantReadMessage);
    return PrintErrorNumber(rs, res);
  }
  return 0;
}


/*  文件压缩测试用例
int MY_CDECL main(int numArgs, const char *args[])
{
  char rs[800] = { 0 };
  int res = main2(numArgs, args, rs);
  fputs(rs, stdout);
  return res;
}*/
