#
# (C) Copyleft 2011
# Late Lee from http://www.latelee.org
#
# A simple way to generate depend file(.depend) for .c & .cpp,
# so you change the head file, it will recompile the 
# file(s) which include the head file.
#

_depend: $(obj).depend

$(obj).depend: $(MAKEROOT)/env.mk $(C_SRCS) $(CPP_SRCS)
	@rm -f $@
	@for f in $(C_SRCS); do \
		g=`basename $$f | sed -e 's/\(.*\)\.\w/\1.o/'`; \
		$(CC) -MM $(CFLAGS) -E -MQ $(MAKEROOT)/obj/$$g $$f >> $@ ; \
	done
	@for f in $(CPP_SRCS); do \
		g=`basename $$f | sed -e 's/\(.*\)\...\w/\1.o/'`; \
		$(CXX) -MM $(CFLAGS) -E -MQ $(MAKEROOT)/obj/$$g $$f >> $@ ; \
	done
