

TARGETS = test_itself_test test_itself_b_test


GTEST_BASE_SRC=googletest
GTEST_INCLUDES=-I. -I${GTEST_BASE_SRC}/googletest/include -I${GTEST_BASE_SRC}/googlemock/include

GTEST_ALL_CXX=${GTEST_BASE_SRC}/build/googlemock/gtest/libgtest.a
GTEST_MAIN=${GTEST_BASE_SRC}/build/googlemock/gtest/libgtest_main.a
GTEST_LIBS=-lpthread

GTEST_DRIVERS=${GTEST_ALL_CXX} ${GTEST_MAIN} ${GTEST_LIBS}


TARGETS_CXX=${TARGETS:%=%.cc} 

def:
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
	rm -f test-all ${TARGETS}

all-from-scratch:
	${MAKE} clean
	${MAKE} clean-googletest
	${MAKE} init
	${MAKE} test-all

.PHONY: test-all
test-all:
	g++ -o test-all -I. -I${GTEST_BASE_SRC} ${TARGETS_CXX} ${GTEST_DRIVERS}
	./test-all


test-each:
	for t in ${TARGETS}; do \
	  ${MAKE} test-one T=$$t; \
	done

test-one:
	g++ -o ${T} -I. -I${GTEST_BASE_SRC} ${T}.cc ${GTEST_DRIVERS}
	./${T}

