Quickstart
==========

This page parses an ``X-Forwarded-For`` header and adapts explicit request data
in the same program.

Program
-------

.. code-block:: cpp

   #include <iostream>

   #include <polycpp/forwarded/forwarded.hpp>

   int main() {
       auto direct = polycpp::forwarded::forwarded(
           "127.0.0.1",
           "10.0.0.2, 10.0.0.1");

       polycpp::forwarded::RequestInfo request;
       request.socket_remote_address = "127.0.0.1";
       request.headers["X-Forwarded-For"] = "10.0.0.2, 10.0.0.1";

       auto adapted = polycpp::forwarded::forwarded(request);

       for (const auto& address : adapted) {
           std::cout << address << '\n';
       }
   }

Expected output
---------------

.. code-block:: text

   127.0.0.1
   10.0.0.1
   10.0.0.2

Key behavior
------------

- The first address is always the socket or connection remote address.
- Header entries are returned from right to left, matching upstream.
- Blank comma entries are skipped.
- ``RequestInfo`` prefers ``socket_remote_address`` over ``connection_remote_address``.
