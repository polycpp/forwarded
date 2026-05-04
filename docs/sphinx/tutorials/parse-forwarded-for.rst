Parse X-Forwarded-For
=====================

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

Use ``RequestInfo`` when porting upstream ``forwarded(req)`` call sites:

.. code-block:: cpp

   polycpp::forwarded::RequestInfo request;
   request.socket_remote_address = "127.0.0.1";
   request.connection_remote_address = "10.0.0.10";
   request.headers.set("X-Forwarded-For", "10.0.0.2, 10.0.0.1");

   auto addresses = polycpp::forwarded::forwarded(request);

``socket_remote_address`` wins over ``connection_remote_address`` because
upstream version 0.2.0 changed to prefer ``req.socket`` over deprecated
``req.connection``.

Use the live polycpp HTTP overload inside a server handler:

.. code-block:: cpp

   auto server = polycpp::http::createServer(ctx,
       [](polycpp::http::IncomingMessage req, polycpp::http::ServerResponse res) {
           auto addresses = polycpp::forwarded::forwarded(req);
           res.end(addresses.empty() ? "" : addresses.front());
       });
