
#include <ihttpd.hpp>
#include <daemon.hpp>

IHTTPD::Daemon::Daemon(const std::string& hostname, ushort port, int tick_msec/*=100*/)
    : hostname_(hostname), port_(port), tick_msec_(tick_msec)
{
    
}

IHTTPD::Daemon::~Daemon()
{
}

