
#pragma once

#include "ihttpd.hpp"
#include "utils.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include <string>

// Handle a connection, request -> response sequence.
//
// Work as HTTP/1.0 Server
//   - Process URI part only.
//   - Ignore header and body.
//   - Send URI as response.
//
// sequence
// - keep reading
//   - read -> find URI -> read out whole header.
// - keep sending
//   - send until all data sent.
// - send shutdown(SD_SEND)
// - keep reading
//   - read out rest data and wait remote shutdown or close.
//     - do not store received data.
// - close local
class IHTTPD::Task
{
public:
    enum {
        READ_BYTES_LIMIT = 1024 * 128, // > whole request bytes
        TIMEOUT_MSEC = 10 * 1000 // > total processing time.
    };

    Task(int sp);
    ~Task();

    bool process();   // process sp. This would block.
    void interrupt(); // Interrupt blocking process and stop thread polling.

protected:
    bool read_uri_();
    bool read_header_();
    bool send_response_();
    bool read_body_();     // and wait close.

    bool poll_(short events); // poll sp_ and intterupt_sp_

    int sp_;
    int interrupt_sp_[2]; // Unix domain socket to awake thread.
    enum { INTR_SP_READ=0, INTR_SP_WRITE=1};

    // processing timeout.
    MsecTimer limit_timer_;

    // request
    std::string read_data_;        // incoming data.

    // response
    std::string respnose_payload_; // data to send.

    uint32_t read_bytes_; // total read bytes.

    friend class IHTTPD::Test::TaskTest;
};
