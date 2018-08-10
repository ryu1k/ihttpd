
#include "gtest/gtest.h"

#include "../src/utils.cc"

using namespace IHTTPD;

TEST(utils, msec2timespec) {
    struct timespec ts;

    uint32_t one_sec = 1000;
    msec2timespec(one_sec, ts);
	ASSERT_EQ(1,    ts.tv_sec);
	ASSERT_EQ(0,    ts.tv_nsec);

    uint32_t msec_max = 0xffffffff;
    msec2timespec(msec_max, ts);
	ASSERT_EQ(msec_max/1000,             ts.tv_sec);
	ASSERT_EQ((msec_max%1000)*1000*1000, ts.tv_nsec);

    uint32_t one_msec = 1;
    msec2timespec(one_msec, ts);
	ASSERT_EQ(0,           ts.tv_sec);
	ASSERT_EQ(1*1000*1000, ts.tv_nsec);
    
    uint32_t zero = 0;
    msec2timespec(zero, ts);
	ASSERT_EQ(0, ts.tv_sec);
	ASSERT_EQ(0, ts.tv_nsec);
}

