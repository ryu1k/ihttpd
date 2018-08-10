
#include "ihttpd.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include <string>

// Handle a connection, request -> response sequence.
class IHTTPD::Worker
{
public:
    Worker();
    ~Worker();
    
    bool process(int sp);

protected:
    bool read_an_unit_();
    bool send_response_();

    int sp_;

    std::string read_data_;
    std::string respnose_payload_;
};
