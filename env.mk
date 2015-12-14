#
#包含了其他makefile都用到的模式规则
#

# cross compile...
CROSS_COMPILE = 

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

 #生成的目标文件名.......
TARGET := wfs  

#顶层目录
ROOT := $(shell pwd)

#lib库
LIB =-lpthread  -lz 

##需要扫描的文件
TARGET_DIRS :=src/ src/common src/tools/lzma src/tools\
				src/thread  \
				src/client  src/slave \
				src/master \
				lib/  lib/error \

#.h 头文件目录
INCLUDE_DIR := \
 	-I$(MAKEROOT)/inc \
	-I$(MAKEROOT)/src/common\
	-I$(MAKEROOT)/src/tools \
	-I$(MAKEROOT)/src/tools/lzma \
	-I$(MAKEROOT)/src/thread \
	-I$(MAKEROOT)/src/master \
	-I$(MAKEROOT)/src/slave  \
	-I$(MAKEROOT)/src/client \

#lib静态库搜索路劲
LIB_DIR := -L$(MAKEROOT)/lib \

CFLAGS := $(INCLUDE_DIR)

CFLAGS += -c -Wall -O2  -D_7ZIP_ST

#对所有的.o文件以.c文件创建它

$(MAKEROOT)/obj/%.o :  %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	${CC} ${CFLAGS} -c $< -o $@ 
	
$(MAKEROOT)/obj/%.o :  %.cpp
	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
	${CXX} ${CFLAGS} -c $< -o $@