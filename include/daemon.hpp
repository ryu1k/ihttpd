
#include "ihttpd.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <string>

// Accept incoming connections and put it to worker.
class IHTTPD::Daemon
{
public:
    Daemon(const std::string& hostname, ushort port, int tick_msec=100);
    ~Daemon();

    bool start(); // start run loop until stop.
    void stop();  

protected:
    bool listen_();
    bool accept_one_();

    int sp_;
    int running_;

    const std::string hostname_;
    const ushort port_;
    const int tick_msec_;
};
