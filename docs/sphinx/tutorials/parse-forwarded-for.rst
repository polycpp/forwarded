Parse X-Forwarded-For
=====================

``forwarded`` preserves the upstream address order but does not make forwarded
addresses trustworthy. Treat ``X-Forwarded-For`` as client-spoofable unless it
is set or overwritten by trusted infrastructure, and apply your own trusted
proxy policy before using returned addresses for authentication, rate limiting,
audit logging, or access control.

Start with the pure overload when the caller already has normalized request
values:

.. code-block:: cpp

   auto addresses = polycpp::forwarded::forwarded(
       "127.0.0.1",
       "10.0.0.2, 10.0.0.1");

   // addresses == {"127.0.0.1", "10.0.0.1", "10.0.0.2"}

Use ``parse_header`` when only the proxy header matters:

.. code-block:: cpp

   auto proxies = polycpp::forwarded::parse_header("10.0.0.2,, 10.0.0.1");
   // proxies == {"10.0.0.1", "10.0.0.2"}

``parse_header`` has no direct remote address. It returns only parsed proxy
entries from the header.

Use ``RequestInfo`` when porting upstream ``forwarded(req)`` call sites:

.. code-block:: cpp

   polycpp::forwarded::RequestInfo request;
   request.socket_remote_address = "127.0.0.1";
   request.connection_remote_address = "10.0.0.10";
   request.headers.set("X-Forwarded-For", "10.0.0.2, 10.0.0.1");

   auto addresses = polycpp::forwarded::forwarded(request);

``socket_remote_address`` wins over ``connection_remote_address`` because
upstream version 0.2.0 changed to prefer ``req.socket`` over deprecated
``req.connection``. If both fields are absent, this overload throws
``polycpp::TypeError``.

Use the live polycpp HTTP overload inside a server handler:

.. code-block:: cpp

   auto server = polycpp::http::createServer(ctx,
       [](polycpp::http::IncomingMessage req, polycpp::http::ServerResponse res) {
           auto addresses = polycpp::forwarded::forwarded(req);
           const auto remote = addresses.empty() || addresses.front().empty()
               ? "unknown"
               : addresses.front();
           res.end(remote);
       });

For a live ``polycpp::http::IncomingMessage``, a missing socket or remote
address produces an empty first address instead of throwing. Check for that
empty value before treating the first entry as a usable remote address.
