

# TARGETS = test_itself_test test_itself_b_test
TARGETS = daemon_test utils_test ihttpd_main_test

OBJS = ${TARGETS:%=%.o}

GTEST_BASE_SRC=googletest
GTEST_INCLUDES=-I. -I../include \
	-I${GTEST_BASE_SRC}/googletest/include -I${GTEST_BASE_SRC}/googlemock/include

GTEST_ALL_CXX=${GTEST_BASE_SRC}/build/googlemock/gtest/libgtest.a
GTEST_MAIN=${GTEST_BASE_SRC}/build/googlemock/gtest/libgtest_main.a
GTEST_LIBS=-lpthread -lboost_timer  -lboost_system

GTEST_CFLAGS=-Wall ${GTEST_INCLUDES}
GTEST_DRIVERS=${GTEST_ALL_CXX} ${GTEST_MAIN} ${GTEST_LIBS}

TARGETS_CXX=${TARGETS:%=%.cc} 

def: test-all


help:
	@echo "targets"
	@echo "  make init  : prepare libgtest.a and so on." 
	@echo "  make clean-googletest : clean libgtest.a and so on."
	@echo ""
	@echo "  test_itself_test : test of googletest itself."
	@echo 

init:
	git clone https://github.com/google/googletest.git
	mkdir googletest/build
	cd googletest/build; \
	  cmake ..; \
	  make

clean-googletest:
	rm -fr googletest

clean:
	rm -f test-all ${TARGETS} ${OBJS}

all-from-scratch:
	${MAKE} clean
	${MAKE} clean-googletest
	${MAKE} init
	${MAKE} test-all

.PHONY: test-all
test-all: clean test

test: build
	./test-all


valgrind-all: clean valgrind

valgrind: build
	valgrind ${VGOPTS} ./test-all

build:  ${OBJS}
	g++ -o test-all ${GTEST_CFLAGS} ${OBJS} ${GTEST_DRIVERS}

%.o: %.cc
	g++ ${GTEST_CFLAGS} ${CXXFLAGS} -c -o $@ $<
