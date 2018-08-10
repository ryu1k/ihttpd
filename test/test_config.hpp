
// Test with sleep or wait.
// These test may take long.
// #define TEST_WITH_WAIT


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
