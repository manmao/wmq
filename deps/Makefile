all:
	cd jemalloc && ./configure --with-jemalloc-prefix=je_ --enable-cc-silence
	cd jemalloc && $(MAKE) lib/libjemalloc.a  #在jemalloc目录下生成lib/libjemalloc.a
