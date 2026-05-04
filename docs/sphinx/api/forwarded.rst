forwarded.hpp
=============

``polycpp::forwarded`` exposes pure parsing helpers plus typed request
adapters for C++ call sites.

.. warning::

   ``X-Forwarded-For`` is client-spoofable unless it is set or overwritten by
   trusted infrastructure. This library parses address lists and does not
   implement trusted proxy policy. Decide which proxy entries are trusted
   before using returned addresses for authentication, rate limiting, audit
   logging, or access control.

Types
-----

.. doxygentypedef:: polycpp::forwarded::AddressList

.. doxygentypedef:: polycpp::forwarded::HeaderMap

.. doxygenstruct:: polycpp::forwarded::RequestInfo
   :members:

Functions
---------

The public functions return address lists in upstream ``forwarded`` order: the
direct remote address first when one is part of the overload, followed by
``X-Forwarded-For`` entries from right to left.

``parse_header``
~~~~~~~~~~~~~~~~

.. code-block:: cpp

   polycpp::forwarded::AddressList
   polycpp::forwarded::parse_header(std::string_view header);

Parses an ``X-Forwarded-For`` header value without adding a direct remote
address. Results contain only proxy entries, ordered from the rightmost header
entry to the leftmost. Blank comma entries are skipped, and leading or trailing
ASCII space around each entry is trimmed.

``forwarded(remote_address, header)``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   polycpp::forwarded::AddressList
   polycpp::forwarded::forwarded(std::string_view remote_address,
                                std::string_view x_forwarded_for = {});

Returns ``remote_address`` as the first element, then appends the
``parse_header(x_forwarded_for)`` result. This overload is pure and uses exactly
the remote address string supplied by the caller; it does not inspect request
objects or apply missing-remote policy.

``forwarded(RequestInfo)``
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   polycpp::forwarded::AddressList
   polycpp::forwarded::forwarded(
       const polycpp::forwarded::RequestInfo& request);

Adapts explicit request data to upstream ``forwarded(req)`` behavior. Header
lookup uses ``request.headers`` and is case-insensitive for
``X-Forwarded-For``. The direct remote address is
``request.socket_remote_address`` when present, otherwise
``request.connection_remote_address``.

The returned list starts with the chosen remote address, then contains parsed
proxy entries from right to left. If both remote address fields are absent, this
overload throws ``polycpp::TypeError``.

``forwarded(IncomingMessage)``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: cpp

   polycpp::forwarded::AddressList
   polycpp::forwarded::forwarded(
       const polycpp::http::IncomingMessage& request);

Adapts a live ``polycpp::http::IncomingMessage``. The overload reads
``request.headers()`` for ``X-Forwarded-For`` and uses ``request.socket()`` for
the direct remote address, falling back to ``request.connection()`` when no
socket handle is present.

If no socket or connection remote address is available from the live request,
the first returned address is an empty string instead of throwing. Parsed proxy
entries, if present, still follow that empty first element.
