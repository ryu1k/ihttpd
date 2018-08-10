
#include "gtest/gtest.h"

#include "test_config.hpp"
#include "../src/utils.cc"

#include <pthread.h>


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

// printf trace
#ifdef TRACE_THREAD
#define TL_(...) printf(__VA_ARGS__)
#else
#define TL_(...)
#endif

#ifdef TEST_WITH_WAIT
static int sleepmsec_shared = 0;
void* sleepmsec_update_shared(void* thread_arg)
{
    uint64_t wait64 = reinterpret_cast<uint64_t>( thread_arg );
    uint32_t wait = static_cast<uint32_t>( wait64 );

    TL_(" %s : wait %d msec : start\n", __func__, wait );
    sleepmsec( wait );
    sleepmsec_shared = wait;
    TL_(" %s : wait %d msec : done\n", __func__, wait);

    return NULL;
}

TEST(utils, sleepmsec_myfirst) {

    pthread_t th;

    uint64_t wait = 100;
    void* thread_arg = reinterpret_cast<void*>( wait );
    ASSERT_EQ(0, pthread_create(&th, NULL, sleepmsec_update_shared, thread_arg) );

    // set by myself.
    sleepmsec_shared = 10;
    sleepmsec(10);
    ASSERT_EQ(10, sleepmsec_shared); // not updated yet.

    // wait
    sleepmsec(200); // will be updated by the thread

    // updated by the thread.
    ASSERT_EQ(wait, static_cast<uint64_t>(sleepmsec_shared));

    // tear down.
    ASSERT_EQ(0, pthread_join(th, NULL));
}


TEST(utils, sleepmsec_theirfirst) {

    pthread_t th;

    uint64_t wait = 10;
    void* thread_arg = reinterpret_cast<void*>( wait );
    ASSERT_EQ(0, pthread_create(&th, NULL, sleepmsec_update_shared, thread_arg) );

    // wait
    sleepmsec(50); // will be updated by the thread

    // updated by the thread.
    ASSERT_EQ(wait, static_cast<uint64_t>(sleepmsec_shared));

    // set by myself.
    sleepmsec_shared = 10;
    ASSERT_EQ(10, sleepmsec_shared);

    sleepmsec(50);

    ASSERT_EQ(10, sleepmsec_shared); // must be my value.

    // tear down.
    ASSERT_EQ(0, pthread_join(th, NULL));
}


TEST(utils, sleepmsec_wait900) {

    pthread_t th;

    uint64_t wait = 900;
    void* thread_arg = reinterpret_cast<void*>( wait );
    ASSERT_EQ(0, pthread_create(&th, NULL, sleepmsec_update_shared, thread_arg) );

    sleepmsec_shared = 10;
    ASSERT_EQ(10, sleepmsec_shared);

    sleep(1);

    // updated by the thread.
    ASSERT_EQ(wait, static_cast<uint64_t>(sleepmsec_shared));

    // tear down.
    ASSERT_EQ(0, pthread_join(th, NULL));
}


TEST(utils, sleepmsec_wait1100) {

    pthread_t th;

    uint64_t wait = 1100;
    void* thread_arg = reinterpret_cast<void*>( wait );
    ASSERT_EQ(0, pthread_create(&th, NULL, sleepmsec_update_shared, thread_arg) );

    sleepmsec_shared = 10;
    ASSERT_EQ(10, sleepmsec_shared);

    sleep(1);

    ASSERT_EQ(10, sleepmsec_shared); // still 10 after 1 sec.

    sleepmsec(200);

    // updated by the thread.
    ASSERT_EQ(wait, static_cast<uint64_t>(sleepmsec_shared));

    // tear down.
    ASSERT_EQ(0, pthread_join(th, NULL));
}

#endif // of #ifdef TEST_DYNAMIC
