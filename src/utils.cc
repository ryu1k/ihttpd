
#include <ihttpd.hpp>

#include <time.h>

static void msec2timespec(uint32_t msec, struct timespec& ts)
{
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec - ts.tv_sec * 1000) * 1000 * 1000;
}


int IHTTPD::sleepmsec(uint32_t msec)
{
    struct timespec ts;
    msec2timespec(msec, ts);

    return nanosleep(&ts, NULL);
}

