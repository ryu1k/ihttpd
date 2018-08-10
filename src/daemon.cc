
#include "ihttpd.hpp"
#include "daemon.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <boost/format.hpp>

#include "utils.hpp"

// #define TRACE_DAEMON_DETAIL
#ifdef TRACE_DAEMON_DETAIL
#undef TRL_
#define TRL_(...) do { \
        fprintf(stderr, "L: %s:%d: ", __func__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
    } while(0)
#endif

IHTTPD::Daemon::Daemon(const std::string& hostname, ushort port, uint tick_msec/*=100*/)
    : sp_(-1), running_(false),
      hostname_(hostname), port_(port), tick_msec_(tick_msec)
{
    
}

IHTTPD::Daemon::~Daemon()
{
    close_();
}

bool IHTTPD::Daemon::run()
{
    running_ = true;

    while( running_ ) {
        // TRL_("working...\n");
        sleepmsec( tick_msec_);
    }
 
    close_();

    return true;
}

void IHTTPD::Daemon::stop()
{
    running_ = false;
}


bool IHTTPD::Daemon::listen_()
{
    TRL_("going to listen. (%s, %d)\n", hostname_.c_str(), port_);

    // resolve name first.
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string strport = (boost::format("%u") % port_).str();
    struct addrinfo* res = NULL;
    int err = getaddrinfo(hostname_.c_str(), strport.c_str(), &hints, &res);
    if (err != 0) {
        // run OK once.
        TRE_("error in getaddrinfo. err=%d. (%s, %s)\n", err, hostname_.c_str(), strport.c_str());
        return false;
    }

    // try to bind to given addresses.
    struct addrinfo* rp;
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        sp_ = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sp_ == -1) {
            continue;
        }

        if (bind(sp_, rp->ai_addr, rp->ai_addrlen) == 0) {
            break; // bounded successfully.
        }

        // bind failed. try another.
        close_(); // close sp_ and reset.
    }

    freeaddrinfo(res);

    if ( rp == NULL) {               /* No address succeeded */
        // run OK once.
        TRE_("can not bind to any addr. (%s, %d)\n", hostname_.c_str(), port_);
        return false;
    }

    // bind suucessful.
    if(-1 == ::listen(sp_, 20) ) {
        TRE_(" failed to listen. errno=%d (%s, %d)\n", errno, hostname_.c_str(), port_);
        close_();
        return false;
    }

    return true;
}

void IHTTPD::Daemon::close_()
{
    if( -1 == sp_ ) {
        // aleady closed.
        return;
    }

    if( -1 == ::close(sp_) ) {
        // executed once.
        TRE_("error on close. errno=%d\n", errno);
        return;
    }

    // close completed.
    sp_ = -1;
}
