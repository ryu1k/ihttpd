

#include "test_config.hpp"

#include "gtest/gtest.h"

#include "daemon.hpp"
#include "../src/ihttpd_main.cc"

#include <pthread.h>
#include <boost/asio.hpp>

using namespace IHTTPD;
using namespace IHTTPD::Test;


#if 1 // def TEST_WITH_WAIT
static void* ihttpd_main_daemon_test_run_daemon(void* thread_arg)
{
    // pthread entry.

    IHTTPD::ihttpd_main(0, NULL);

    pthread_exit(NULL);
    return NULL;
}

TEST(ihttpd_main, signal_ignored) {
    // wait a while daemon work tick.
    auto wait = [](int times=1) { 
        sleepmsec( Daemon::DEFAULT_TICK_MSEC * times);
    };
    auto is_running = []() {
        return get_the_daemon() && get_the_daemon()->is_running();
    };

    // run and exit when stopped.
    const std::string host("127.0.0.1");
    const ushort port = 56789;
    Daemon daemon(host, port);

    pthread_t th;
    ASSERT_EQ(0, pthread_create(&th, NULL, ihttpd_main_daemon_test_run_daemon, NULL) );
    wait(); // wait full startup.
    ASSERT_EQ(true, is_running());

    // ignored signal
    ASSERT_EQ(0, kill(getpid(), SIGPIPE)); // must be ignored.
    wait(1); // wait enough
    ASSERT_EQ(true, is_running()); // must keep running

    get_the_daemon()->stop(); // stop once.
    wait(2); // wait enough
    ASSERT_EQ(false, is_running()); // must keep running

    // tear down.
    ASSERT_EQ(0, pthread_join(th, NULL));
}


TEST(ihttpd_main, signal_terminate) {
    // wait a while daemon work tick.
    auto wait = [](int times=1) { 
        sleepmsec( Daemon::DEFAULT_TICK_MSEC * times);
    };
    auto is_running = []() {
        return get_the_daemon() && get_the_daemon()->is_running();
    };

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
        wait(); // wait full startup.
        ASSERT_EQ(true, is_running());

        ASSERT_EQ(0, kill(getpid(), sig)); // must be ignored.
        wait(2); // wait enough
        ASSERT_EQ(false, is_running()); // must be stopped.

        // tear down.
        ASSERT_EQ(0, pthread_join(th, NULL));
    }
}
#endif // of #ifdef TEST_WITH_WAIT


