

CXXSRCS = ihttpd ihttpd_main utils daemon

OBJS = ${CXXSRCS:%=%.o}

INCLUDES = -I../include
LIBS = -lboost_timer
CXXFLAGS = -Wall ${INCLUDES}

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


