LIBUV_BRANCH=v1.7.1

all: jemalloc libuv
.PHONY : all

jemalloc:
	cd jemalloc && ./configure --with-jemalloc-prefix=je_ --enable-cc-silence
	cd jemalloc && $(MAKE) lib/libjemalloc.a 
.PHONY : jemalloc

libuv_build:
	cd libuv && sh autogen.sh
	cd libuv && ./configure
	cd libuv && make
	cp libuv/.libs/libuv.a libuv/
.PHONY : libuv_build

libuv_fetch:
	if test -e libuv; \
	then cd libuv && git pull origin $(LIBUV_BRANCH); \
	else git clone https://github.com/libuv/libuv libuv; \
	fi
	cd libuv && git checkout $(LIBUV_BRANCH)
.PHONY : libuv_fetch

libuv: libuv_fetch libuv_build
.PHONY : libuv
	
