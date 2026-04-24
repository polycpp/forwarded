Request Adapter Boundary
========================

Upstream ``forwarded(req)`` reads these dynamic properties:

- ``req.headers['x-forwarded-for']``
- ``req.socket.remoteAddress``
- ``req.connection.remoteAddress``

The C++ port does not emulate JavaScript duck typing. Use ``RequestInfo`` to
make the boundary explicit:

.. code-block:: cpp

   polycpp::forwarded::RequestInfo request;
   request.headers["X-Forwarded-For"] = "10.0.0.2, 10.0.0.1";
   request.socket_remote_address = "127.0.0.1";

   auto addresses = polycpp::forwarded::forwarded(request);

Header lookup is case-insensitive for C++ convenience. This is a deliberate
adaptation; Node.js normally exposes incoming header names in lowercase.

If both remote address fields are missing, ``forwarded(request)`` throws
``polycpp::TypeError``.
