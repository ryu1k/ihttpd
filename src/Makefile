

CXXSRCS = daemon ihttpd ihttpd_main task utils

OBJS = ${CXXSRCS:%=%.o}

INCLUDES = -I../include
LIBS = -lboost_timer
CXXFLAGS = -Wall -g ${INCLUDES}

make: ihttpd

remake: clean ihttpd

help:
	@echo "ihttp makefile"

clean:
	rm -f ihttpd ${OBJS}

ihttpd: ${OBJS}
	g++ ${CXXFLAGS} -o $@ ${OBJS} ${LIBS}

%.o: %.cc ../include/%.hpp
	g++ ${CXXFLAGS} -c -o $@ $< 


