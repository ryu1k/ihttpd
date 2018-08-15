

#ifndef TEST_CONFIG_HPP_INCL_
#define TEST_CONFIG_HPP_INCL_

#include "ihttpd.hpp"
#include "daemon.hpp"


////////////////////////////////////////////////////////////////////////
// Test conditions.

// Test with sleep or wait.
// These test may take long.
//  !! : These test may be affected by the system load
//       even though IS_AFFECTED_BY_LOAD is not applied.
#define TEST_WITH_WAIT

// Test which may be affected by system load.
// These test may fail under the high system load.
#define TEST_AFFECTED_BY_LOAD


// Show trace output from test thread.
// #define TRACE_THREAD


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

#endif // of TEST_CONFIG_HPP_INCL_
