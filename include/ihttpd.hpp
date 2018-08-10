
#include <stdint.h>

namespace IHTTPD {
    class Daemon; // Handle incoming connection and pass it to worker.
    class Worker; // Process single connection from request to response.

    // for test
    namespace Test {
        class DaemonTest;
    };

    // in utils.cc
    int sleepmsec(uint32_t msec); // sleep mili-seconds.
};

