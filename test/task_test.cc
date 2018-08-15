
#include "gtest/gtest.h"

#include "../src/task.cc"

#include "test_config.hpp"

#include <pthread.h>
#include <boost/asio.hpp>

using namespace IHTTPD;
using namespace IHTTPD::Test;

class IHTTPD::Test::TaskTest : public ::testing::Test
{
public:
    static void constrcutor();
    static void process();
    static void interrupt();
    static void read_uri_();
    static void read_header_();
    static void send_response_();
    static void read_body_();
    static void poll_();
};


// test of daemon parameter, parameter is applied correctly or not.
TEST(TaskTest, constrcutor) {
    TaskTest::constrcutor();
}
void TaskTest::constrcutor()
{
}
