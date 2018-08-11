
#pragma once

#include "ihttpd.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include <string>

// Accept incoming connections and put it to worker.
class IHTTPD::Daemon
{
public:
    enum { DEFAULT_TICK_MSEC=100 };

    Daemon(const std::string& hostname, ushort port, uint tick_msec=DEFAULT_TICK_MSEC);
    ~Daemon();

    // entry point. keep running until stop() or error
    bool run();

    // control API
    void stop();

    // status query
    bool is_running() const;

protected:
    bool listen_();
    void close_();

    bool accept_one();
    bool process_one(int newsp);

    int sp_;
    bool running_;

    const std::string hostname_;
    const ushort port_;
    const uint tick_msec_;

    friend IHTTPD::Test::DaemonTest;
};
