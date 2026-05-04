Quickstart
==========

This page builds the same address list from normalized values and from explicit
request data.

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
       request.headers.set("X-Forwarded-For", "10.0.0.2, 10.0.0.1");

       auto adapted = polycpp::forwarded::forwarded(request);

       if (direct != adapted) {
           return 1;
       }

       for (const auto& address : direct) {
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

- Header entries are returned from right to left, matching upstream.
- Blank comma entries are skipped.
- ``RequestInfo`` prefers ``socket_remote_address`` over ``connection_remote_address``.
- ``forwarded(request)`` throws ``polycpp::TypeError`` if both remote address
  fields are absent.

Which API should I use?
-----------------------

.. list-table::
   :header-rows: 1

   * - Situation
     - API
     - Missing remote-address behavior
   * - You only need to parse an ``X-Forwarded-For`` header.
     - ``parse_header(header)``
     - No remote address is involved; the result contains only header entries.
   * - You already have a normalized remote address string.
     - ``forwarded(remote_address, header)``
     - The supplied string is returned exactly as the first entry.
   * - You are adapting Node-style request data in C++.
     - ``forwarded(RequestInfo)``
     - ``socket_remote_address`` wins, ``connection_remote_address`` falls back, and missing both throws ``polycpp::TypeError``.
   * - You are inside a ``polycpp::http`` handler.
     - ``forwarded(IncomingMessage)``
     - Missing socket or remote address returns an empty first entry.

Trust boundary
--------------

``X-Forwarded-For`` is client-spoofable unless trusted infrastructure sets or
overwrites it. ``forwarded`` parses and orders values only; it does not decide
which proxy entries are trusted. Make that decision before using an address for
authentication, rate limiting, audit logging, or access control.
