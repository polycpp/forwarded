Request Adapter Boundary
========================

``forwarded`` parses address lists only. ``X-Forwarded-For`` can be supplied by
clients unless trusted infrastructure sets or overwrites it, so callers must
choose which proxy entries are trusted before using any returned address for
authentication, rate limiting, audit logging, or access control.

Upstream ``forwarded(req)`` reads these dynamic properties:

- ``req.headers['x-forwarded-for']``
- ``req.socket.remoteAddress``
- ``req.connection.remoteAddress``

The C++ port does not emulate JavaScript duck typing. Use ``RequestInfo`` to
make the boundary explicit:

.. code-block:: cpp

   polycpp::forwarded::RequestInfo request;
   request.headers.set("X-Forwarded-For", "10.0.0.2, 10.0.0.1");
   request.socket_remote_address = "127.0.0.1";

   auto addresses = polycpp::forwarded::forwarded(request);

Header lookup is case-insensitive for C++ convenience. This is a deliberate
adaptation; Node.js normally exposes incoming header names in lowercase.

If both remote address fields are missing, ``forwarded(request)`` throws
``polycpp::TypeError``.

When the caller already has a live polycpp HTTP request, pass it directly:

.. code-block:: cpp

   auto addresses = polycpp::forwarded::forwarded(incoming_message);

The live overload reads ``incoming_message.headers()`` and uses
``incoming_message.socket()`` first, falling back to
``incoming_message.connection()`` when no socket handle is present. If the live
request has no socket or connection remote address, the first returned address
is an empty string.
