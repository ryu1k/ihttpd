
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <time.h>

namespace IHTTPD {
    class Daemon; // Handle incoming connection and pass it to worker.
    class Task;   // Process single connection from request to response.
    class Worker; // Process a task generated by the Daemon.

    // for test
    namespace Test {
        class DaemonTest;
        class TaskTest;
        class MsecTimerTest;
    };

    // in ihttpd_main.cc
    int ihttpd_main(int argc, const char** argv);
    IHTTPD::Daemon* get_the_daemon();

    // in utils.cc
    class MsecTimer;

    void msec2timespec(uint32_t msec, struct timespec* ts);
    int sleepmsec(uint32_t msec); // sleep mili-seconds.
};

// global trace.

////////////////////////////////////////////////////////////////////////
// local trace.
//   no trace as default
#ifndef TRL_
#define TRL_(...)
#endif

// if you want to trace, use following
#undef TRL_
#define TRL_(...) do { fprintf(stderr, "L: %s:%d: ", __func__, __LINE__); fprintf(stderr, __VA_ARGS__); } while(0)

// after trace, use following to disable again.
#undef TRL_
#define TRL_(...)

#define TRI_(...) do { \
        fprintf(stderr, "I: %s:%d: ", __func__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
    } while(0)
#define TRE_(...) do { \
        fprintf(stderr, "E: %s:%d: ", __func__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
    } while(0)
