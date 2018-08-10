
#include "ihttpd.hpp"
#include "utils.hpp"

#include <time.h>
#include <unistd.h>

void IHTTPD::msec2timespec(uint32_t msec, struct timespec* ts_)
{
    struct timespec& ts = *ts_;
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec - ts.tv_sec * 1000) * 1000 * 1000;
}


int IHTTPD::sleepmsec(uint32_t msec)
{
    struct timespec ts;
    msec2timespec(msec, &ts);

    return nanosleep(&ts, NULL);
}

uint32_t IHTTPD::MsecTimer::now()
{
    return static_cast<uint32_t>( elapsed().wall / (1000 * 1000) );
}
