# Instant HTTP Daemon (ihttpd)

Instant HTTP server to study C/C++, multi threading by pthread and BSD socket.
To learn low level programing, use of boost or other high level libraries is minimum.

# Architecture

* Daemon
  * Listen a socket and accept.
  * Stop on signal or error.
  * Accept incoming connection and generate "Task".
  * Send "Task" to worker in multi threaded mode.

* Task (Not implemeted yet)
  * Represent a connection.
  * Parse request.
  * Process content.
  * Send response.

* Worker (Not implemeted yet)
  * Threading worker.
  * Wait a "Task" from the Daemon.
  * Receive a "Task" and process it.

# Limitatinos

* One thread can handle one connection only. This limits scalablility.
* Lacks c++ exception handling.
  * Do not handle bad_alloc.

