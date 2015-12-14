#
#当前目录下是C文件
#
#
include $(MAKEROOT)/env.mk 			#包括头文件，执行模式规则

C_SRCS := $(wildcard *.c)
CPP_SRCS :=$(wildcard *.cpp)

C_OBJS := $(addprefix $(MAKEROOT)/obj/,$(patsubst %.c,%.o,$(C_SRCS)))
CPP_OBJS := $(addprefix $(MAKEROOT)/obj/,$(patsubst %.cpp,%.o,$(CPP_SRCS)))

all: $(obj).depend $(C_OBJS)  $(CPP_OBJS) 

.PHONY: all clean

clean:
	rm -rf .depend
#########################################################################
include $(MAKEROOT)/rules.mk

sinclude $(obj).depend

#########################################################################