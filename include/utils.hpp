
#pragma once

#include "ihttpd.hpp"

#include <time.h>
#include <boost/timer/timer.hpp>

class IHTTPD::MsecTimer : public boost::timer::cpu_timer
{
public:
    uint32_t now();
};
