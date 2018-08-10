
#include "gtest/gtest.h"

#include "../src/daemon.cc"

using namespace IHTTPD::Test;

class IHTTPD::Test::DaemonTest : public ::testing::Test
{
public:
    static void Constrcutor();
};


TEST(DaemonTest, Constrcutor) {
    DaemonTest::Constrcutor();
}
void DaemonTest::Constrcutor()
{
    const std::string host("test-host-name");
    const ushort port = 56789;
    int tick = 50;

    // Is value set exactly ?
    IHTTPD::Daemon daemon1(host, port, tick);
	ASSERT_EQ(host, daemon1.hostname_);
	ASSERT_EQ(port, daemon1.port_);
	ASSERT_EQ(tick, daemon1.tick_msec_);

    // Check defalut value.
    IHTTPD::Daemon daemon2(host, port);
	ASSERT_EQ(host, daemon2.hostname_);
	ASSERT_EQ(port, daemon2.port_);
	ASSERT_EQ(daemon2.DEFAULT_TICK_MSEC, daemon2.tick_msec_); 
}
