
#include "gtest/gtest.h"

#include "../src/daemon.cc"

#include "test_config.hpp"

#include <pthread.h>
#include <boost/asio.hpp>

using namespace IHTTPD;
using namespace IHTTPD::Test;

class IHTTPD::Test::DaemonTest : public ::testing::Test
{
public:
    static void constrcutor();
    static void run_stop();
    static void listen_();
    static void accept_();
    static void close_();
};

TEST(DaemonTest, constrcutor) {
    DaemonTest::constrcutor();
}
void DaemonTest::constrcutor()
{
    // constrcutor must keep configured value.

    const std::string host("test-host-name");
    const ushort port = 56789;
    uint tick = 50;

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

#ifdef TEST_WITH_WAIT
static void* daemon_test_run_daemon(void* thread_arg)
{
    Daemon& daemon = *reinterpret_cast<Daemon*>(thread_arg);
    daemon.run();

    return NULL;
}

TEST(DaemonTest, run_stop) {
    DaemonTest::run_stop();
}
void DaemonTest::run_stop()
{
    // run and exit when stopped.
    const std::string host("127.0.0.1");
    const ushort port = 56789;

    Daemon daemon(host, port);

    ASSERT_EQ(false, daemon.running_);

    pthread_t th;
    ASSERT_EQ(0, pthread_create(&th, NULL, daemon_test_run_daemon, &daemon) );

    sleepmsec(daemon.tick_msec_ * 2);
    ASSERT_EQ(true, daemon.running_);

    daemon.stop();

    sleepmsec(daemon.tick_msec_ * 2);  // wait enough
    ASSERT_EQ(false, daemon.running_); // must not be running

    ASSERT_EQ(0, pthread_join(th, NULL));
}
#endif // of #ifdef TEST_WITH_WAIT


// #undef TRL_
// #define TRL_(...) do { fprintf(stderr, "L: %s:%d: ", __func__, __LINE__); fprintf(stderr, __VA_ARGS__); } while(0)

TEST(DaemonTest, listen_) {
    DaemonTest::listen_();
}
void DaemonTest::listen_()
{
    // ASSERT_FALSE(true) << "!!! set nonblock !!!";

    {
        TRL_("good argument.\n");
        Daemon daemon("127.0.0.1", 56789);
        ASSERT_EQ(-1, daemon.sp_);
        ASSERT_EQ(true, daemon.listen_());
        ASSERT_NE(-1, daemon.sp_);
    }

    {
        TRL_("good argument. previous resource must be closed.\n");
        Daemon daemon("127.0.0.1", 56789);
        ASSERT_EQ(-1, daemon.sp_);
        ASSERT_EQ(true, daemon.listen_());
        ASSERT_NE(-1, daemon.sp_);
    }

    {
        TRL_("not numeric\n");
        Daemon daemon("localhost", 45678);
        ASSERT_EQ(-1, daemon.sp_);
        ASSERT_EQ(true, daemon.listen_());
        ASSERT_NE(-1, daemon.sp_);
    }

    {
        TRL_("invalid addr.\n");
        Daemon daemon("8.8.8.8", 0); // must fail.
        ASSERT_EQ(-1, daemon.sp_);
        ASSERT_EQ(false, daemon.listen_());
        ASSERT_EQ(-1, daemon.sp_);
    }

    {
        TRL_("check opening of the por indirectly by opening the port twice.\n");
        Daemon daemon("127.0.0.1", 56789);
        ASSERT_EQ(-1, daemon.sp_);
        ASSERT_EQ(true, daemon.listen_());
        ASSERT_NE(-1, daemon.sp_);

        Daemon daemon2("127.0.0.1", 56789);
        ASSERT_EQ(-1, daemon2.sp_);
        ASSERT_EQ(false, daemon2.listen_()); // must fail
        ASSERT_EQ(-1, daemon2.sp_);
    }

#ifdef TEST_WITH_WAIT
    {
        TRL_("port closed after stop.\n");
        Daemon daemon("127.0.0.1", 56789);
        ASSERT_EQ(-1, daemon.sp_);
        ASSERT_EQ(true, daemon.listen_());
        ASSERT_NE(-1, daemon.sp_);

        pthread_t th;
        ASSERT_EQ(0, pthread_create(&th, NULL, daemon_test_run_daemon, &daemon) );
        sleepmsec(daemon.tick_msec_); // wait startup

        Daemon daemon2("127.0.0.1", 56789);
        ASSERT_EQ(-1, daemon2.sp_);
        ASSERT_EQ(false, daemon2.listen_()); // fail to re-open.
        ASSERT_EQ(-1, daemon2.sp_);

        daemon.stop(); // closed by stop.
        sleepmsec(daemon.tick_msec_ * 3); // wait complete of stop.
        ASSERT_EQ(-1, daemon.sp_); // must be closed.

        Daemon daemon3("127.0.0.1", 56789);
        ASSERT_EQ(-1, daemon3.sp_);
        ASSERT_EQ(true, daemon3.listen_()); // must be able to re-open.
        ASSERT_NE(-1, daemon3.sp_);
    }
#endif // of #ifdef TEST_WITH_WAIT
}
#undef TRL_
#define TRL_(...)

#ifdef TEST_WITH_WAIT
static void* DaemonTest_accpet_accessor(void* arg_)
{
    // connect must be successful.
    namespace asio = boost::asio;
    asio::io_service io_service;
    asio::ip::tcp::socket socket(io_service);
    boost::system::error_code error;

    socket.connect(asio::ip::tcp::endpoint( asio::ip::address::from_string("127.0.0.1"), 56789),
                   error);
    sleepmsec(100);

    uint64_t is_ok = ! error;
    TRI_(" connect is_ok=%lu\n", is_ok);

    pthread_exit( reinterpret_cast<void*>(is_ok) );
    return NULL;
}


TEST(DaemonTest, accept_) {
    DaemonTest::accept_();
}
void DaemonTest::accept_()
{
    Daemon daemon("127.0.0.1", 56789);
    ASSERT_TRUE( daemon.listen_() );

    // no connection.
    MsecTimer timer;
    ASSERT_EQ(true, daemon.accept_one());
    ASSERT_EQ(true, daemon.accept_one());

    // tick * 1.8 < elapsed < tick * 2.2
    ASSERT_LT(static_cast<uint>(daemon.tick_msec_ * 2 * 0.9), timer.now());
    ASSERT_GT(static_cast<uint>(daemon.tick_msec_ * 2 * 1.1), timer.now());

    // launch accessor.
    pthread_t th;
    ASSERT_EQ(0, pthread_create(&th, NULL, DaemonTest_accpet_accessor, NULL));

    // need successful accept.
    timer.start();
    ASSERT_EQ(true, daemon.accept_one());

    // must have completed in 50 msec.
    ASSERT_GT(static_cast<uint>(50), timer.now());

    // connect to daemon by thread must be successful.
    void* retval = NULL;
    ASSERT_EQ(0, pthread_join(th, &retval));
    ASSERT_EQ(1, static_cast<int>(reinterpret_cast<uint64_t>(retval)));
}
#endif // of #ifdef TEST_WITH_WAIT


TEST(DaemonTest, close_) {
    DaemonTest::close_();
}
void DaemonTest::close_()
{
    Daemon daemon("127.0.0.1", 56789);

    // must be invalid first.
    ASSERT_EQ(-1, daemon.sp_);

    // assign.
    daemon.sp_ = socket(AF_INET, SOCK_STREAM, 0);

    // some sockeet must be assigned.
    ASSERT_NE(-1, daemon.sp_);

    // socket closed and marked as invalid.
    daemon.close_();
    ASSERT_EQ(-1, daemon.sp_);

    // must be able to close twice.
    daemon.close_();
    ASSERT_EQ(-1, daemon.sp_);
}
