#
#当前目录下是C文件
#
#
include $(MAKEROOT)/env.mk #包括头文件，执行模式规则
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS)) #生成所有.c文件对应的.o文件
#OBJS +=thrmgr.o
.PHONY: all
all: $(OBJS)