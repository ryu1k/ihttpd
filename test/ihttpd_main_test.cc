

#include "test_config.hpp"

#include "gtest/gtest.h"

#include "daemon.hpp"
#include "../src/ihttpd_main.cc"

#include <pthread.h>
#include <boost/asio.hpp>

using namespace IHTTPD;
using namespace IHTTPD::Test;

#ifdef TEST_WITH_WAIT

////////////////////////////////////////////////////////////////////////
// Utility methods

// theDaemon_ is running or not.
static auto is_the_daemon_running = []() {
    return IHTTPD::get_the_daemon() && IHTTPD::get_the_daemon()->is_running();
};

// wait a while daemon work tick.
static auto wait_daemon_ticks = [](int times) {
    IHTTPD::sleepmsec( IHTTPD::Daemon::DEFAULT_TICK_MSEC * times);
};


////////////////////////////////////////////////////////////////////////
// Tests

//  Is ignore target signal ignroed
static void* ihttpd_main_daemon_test_run_daemon(void* thread_arg);
TEST(ihttpd_main, signal_ignored) {
    // run and exit when stopped.
    const std::string host("127.0.0.1");
    const ushort port = 56789;
    Daemon daemon(host, port);

    pthread_t th;
    ASSERT_EQ(0, pthread_create(&th, NULL, ihttpd_main_daemon_test_run_daemon, NULL) );
    wait_daemon_ticks(1); // wait full startup.
    ASSERT_EQ(true, is_the_daemon_running());

    // ignored signal
    ASSERT_EQ(0, kill(getpid(), SIGPIPE)); // must be ignored.
    wait_daemon_ticks(1); // wait enough
    ASSERT_EQ(true, is_the_daemon_running()); // must keep running

    get_the_daemon()->stop(); // stop once.
    wait_daemon_ticks(2); // wait enough
    ASSERT_EQ(false, is_the_daemon_running()); // must keep running

    // tear down.
    ASSERT_EQ(0, pthread_join(th, NULL));
}

//  Is accept target signal will cause stop of daemon or not.
static void* ihttpd_main_daemon_test_run_daemon(void* thread_arg);
TEST(ihttpd_main, signal_terminate) {
    // run and exit when stopped.
    const std::string host("127.0.0.1");
    const ushort port = 56789;
    Daemon daemon(host, port);

    // terminate signals
    int signals[] = { SIGHUP, SIGINT, SIGTERM }; 
    for( uint i=0; i<sizeof(signals)/sizeof(int); i++) {
        int sig = signals[i];
        TRI_(" try to stop by signum=%d\n", sig);

        pthread_t th;
        ASSERT_EQ(0, pthread_create(&th, NULL, ihttpd_main_daemon_test_run_daemon, &daemon) );
        wait_daemon_ticks(1); // wait full startup.
        ASSERT_EQ(true, is_the_daemon_running());

        ASSERT_EQ(0, kill(getpid(), sig)); // must be ignored.
        wait_daemon_ticks(2); // wait enough
        ASSERT_EQ(false, is_the_daemon_running()); // must be stopped.

        // tear down.
        ASSERT_EQ(0, pthread_join(th, NULL));
    }
}

static void* ihttpd_main_daemon_test_run_daemon(void* thread_arg)
{
    // pthread entry.

    IHTTPD::ihttpd_main(0, NULL);

    pthread_exit(NULL);
    return NULL;
}

#endif // of #ifdef TEST_WITH_WAIT


