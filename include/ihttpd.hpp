

#include "gtest/gtest.h"

namespace IHTTPD {
    class Daemon; // Handle incoming connection and pass it to worker.
    class Worker; // Process single connection from request to response.

    // for test
    namespace Test {
        class DaemonTest;
    };
};


