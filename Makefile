export MAKEROOT := $(shell pwd)

include ./env.mk

define build_obj
for SubDir in $(TARGET_DIRS); do \
    if ! [ -d $$SubDir ]; then \
        echo "The $$SubDir is not exist !"; \
        exit 11; \
    fi; \
    echo "Building $$SubDir ..."; \
    make -C $$SubDir ; \
    if [ $$? -ne 0 ]; then \
echo "Building $$SubDir is failed !"; \
        exit 12; \
    fi; \
done
endef


all :
	@$(call build_obj) #调用过程
	#生成最终目标文件
	gcc -fPIC -o $(MAKEROOT)/bin/$(TARGET) $(MAKEROOT)/obj/*.o  $(LIB) $(LIB_DIR)
clean :
	-rm $(MAKEROOT)/obj/*.o  $(MAKEROOT)/bin/$(TARGET) 
