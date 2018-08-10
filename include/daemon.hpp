
#include "ihttpd.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include <string>

// Accept incoming connections and put it to worker.
class IHTTPD::Daemon
{
    enum { DEFAULT_TICK_MSEC=100 };

public:
    Daemon(const std::string& hostname, ushort port, int tick_msec=DEFAULT_TICK_MSEC);
    ~Daemon();

    bool run(); // keep running until stop() or error()
    void stop();

protected:
    bool listen_();
    bool accept_one();
    void close_();

    int sp_;
    bool running_;

    const std::string hostname_;
    const ushort port_;
    const int tick_msec_;

    friend IHTTPD::Test::DaemonTest;
};
