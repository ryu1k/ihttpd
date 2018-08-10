
#include "ihttpd.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include <string>

// Accept incoming connections and put it to worker.
class IHTTPD::Daemon
{
    enum { DEFAULT_TICK_MSEC=100 };

public:
    Daemon(const std::string& hostname, ushort port, uint tick_msec=DEFAULT_TICK_MSEC);
    ~Daemon();

    bool run(); // keep running until stop() or error()
    void stop();

protected:
    bool listen_();
    void close_();

    bool accept_one();
    bool process_one();

    int sp_;
    bool running_;

    const std::string hostname_;
    const ushort port_;
    const uint tick_msec_;

    friend IHTTPD::Test::DaemonTest;
};
