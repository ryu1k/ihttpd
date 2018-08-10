

CXXSRCS = ihttpd

OBJS = ${CXXSRCS:%=%.o}

INCLUDES = -I../include

CXXFLAGS = -Wall ${INCLUDES}

make: ihttpd

remake: clean ihttpd

help:
	@echo "ihttp makefile"

clean:
	rm -f ihttpd ${OBJS}

ihttpd: ${OBJS}
	g++ ${CXXFLAGS} -o $@ $<

%.o: %.cc ../include/%.hpp
	g++ ${CXXFLAGS} -c -o $@ $< 


