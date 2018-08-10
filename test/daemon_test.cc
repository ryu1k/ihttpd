
#include "gtest/gtest.h"

#include "../src/daemon.cc"

#include "test_config.hpp"

#include <pthread.h>

using namespace IHTTPD;
using namespace IHTTPD::Test;

class IHTTPD::Test::DaemonTest : public ::testing::Test
{
public:
    static void Constrcutor();
    static void Run_Stop();
};

TEST(DaemonTest, Constrcutor) {
    DaemonTest::Constrcutor();
}
void DaemonTest::Constrcutor()
{
    // constrcutor must keep configured value.

    const std::string host("test-host-name");
    const ushort port = 56789;
    int tick = 50;

    // Is value set exactly ?
    Daemon daemon1(host, port, tick);

	ASSERT_EQ(-1,    daemon1.sp_);
	ASSERT_EQ(false, daemon1.running_);

	ASSERT_EQ(host,  daemon1.hostname_);
	ASSERT_EQ(port,  daemon1.port_);
	ASSERT_EQ(tick,  daemon1.tick_msec_);

    // Check defalut value.
    Daemon daemon2(host, port);
	ASSERT_EQ(host, daemon2.hostname_);
	ASSERT_EQ(port, daemon2.port_);
	ASSERT_EQ(daemon2.DEFAULT_TICK_MSEC, daemon2.tick_msec_); 
}


void* daemon_test_run_daemon(void* thread_arg)
{
    Daemon& daemon = *reinterpret_cast<Daemon*>(thread_arg);
    daemon.run();

    return NULL;
}


TEST(DaemonTest, Run_Stop) {
    DaemonTest::Run_Stop();
}
void DaemonTest::Run_Stop()
{
    // run and exit when stopped.
    const std::string host("test-host-name");
    const ushort port = 56789;

    Daemon daemon(host, port);

    ASSERT_EQ(false, daemon.running_);

    pthread_t th;
    // daemon.stop() will be called after 200 msec.
    ASSERT_EQ(0, pthread_create(&th, NULL, daemon_test_run_daemon, &daemon) );

    sleepmsec(daemon.tick_msec_ * 2);
    ASSERT_EQ(true, daemon.running_);

    daemon.stop();

    sleepmsec(daemon.tick_msec_ * 2);  // wait enough
    ASSERT_EQ(false, daemon.running_); // must not be running

    ASSERT_EQ(0, pthread_join(th, NULL));
}
