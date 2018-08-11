
#include "ihttpd.hpp"

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "daemon.hpp"

// forward declarations.
static bool set_sighandlers();
static bool unset_sighandlers();

static IHTTPD::Daemon* theDaemon_ = NULL;

IHTTPD::Daemon* IHTTPD::get_the_daemon()
{
    return theDaemon_;
}

int IHTTPD::ihttpd_main(int argc, const char** argv)
{
    // assign daemon
    theDaemon_ = new IHTTPD::Daemon("127.0.0.1", 56789);

    do {
        // set sighanlers.
        if( ! set_sighandlers() ) {
            TRE_("failed to set signal hander. exit.\n");
            break;
        }
        
        // run
        TRI_("Daemon start now. ihttpd pid=%d. Send SIGHUP or press Ctrl-C to stop.\n", getpid());
        theDaemon_->run();
        
        // reset sighandlers.
        unset_sighandlers();
    } while(0);

    // release daemon
    delete theDaemon_;
    theDaemon_ = NULL;

    return 0;
}


////////////////////////////////////////////////////////////////////////
// signal handling

static const int signal_ignore [] = { SIGPIPE };
static const int signal_terminate [] = { SIGHUP, SIGINT, SIGTERM };
using ihttpd_sigaction_proc_t = void (*)(int signum, siginfo_t* siginfo, void* p) ;

static void ihttpd_sigaction_proc(int signum, siginfo_t* siginfo, void* p)
{
    TRI_(" signal comes. signum=%d. going to stop.\n", signum);
    if( theDaemon_ ) {
        theDaemon_->stop();
    } else {
        TRE_(" strange. theDaemon_ seems NULL.\n");
    }
    return;
}


static bool set_sighandler_one(int signum, ihttpd_sigaction_proc_t proc)
{
    struct sigaction act = {};
    struct sigaction prev = {};
    
    act.sa_sigaction = proc;
    if( -1 == sigfillset(&act.sa_mask) ) { // block all signals in handler.
        TRE_(" error on sigfillset. errno=%d\n", errno);
        return false;
    }
    act.sa_flags = SA_SIGINFO;
    if( -1 == sigaction(signum, &act, &prev) ) {
        TRE_(" error on sigaction. errno=%d\n", errno);
        return false;
    }

    return true;
}

static bool set_sighandlers()
{
    // ignore
    for( uint i=0; i<sizeof(signal_ignore)/sizeof(int); i++) {
        if( ! set_sighandler_one( signal_ignore[i], 
                                  reinterpret_cast<ihttpd_sigaction_proc_t>(SIG_IGN)) ) {
            return false;
        }
    }

    // stop daemon.
    for( uint i=0; i<sizeof(signal_terminate)/sizeof(int); i++) {
        if( ! set_sighandler_one( signal_terminate[i], ihttpd_sigaction_proc) ) {
            return false;
        }
    }
    
    TRI_("sighandler is set successfully.\n");
    return true;
}

static bool unset_sighandlers()
{
    // ignore
    for( uint i=0; i<sizeof(signal_ignore)/sizeof(int); i++) {
        if( ! set_sighandler_one( signal_ignore[i], 
                                  reinterpret_cast<ihttpd_sigaction_proc_t>(SIG_DFL)) ) {
            return false;
        }
    }

    // stop daemon.
    for( uint i=0; i<sizeof(signal_terminate)/sizeof(int); i++) {
        if( ! set_sighandler_one( signal_ignore[i], 
                                  reinterpret_cast<ihttpd_sigaction_proc_t>(SIG_DFL)) ) {
            return false;
        }
    }
    
    TRI_("sighandler is unset successfully.\n");
    return true;
}
