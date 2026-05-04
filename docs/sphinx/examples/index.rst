Examples
========

Runnable programs under ``examples/`` use only the public API.

``parse_header`` and ``forwarded(remote_address, header)`` are different APIs:
``parse_header`` returns only header entries, while the remote/header example
below prepends the supplied remote address.

.. toctree::
   :maxdepth: 1

   parse-header
   request-adapter

Running examples
----------------

From the repository root:

.. code-block:: bash

   cmake -B build -G Ninja -DPOLYCPP_FORWARDED_BUILD_EXAMPLES=ON
   cmake --build build --target polycpp_forwarded_example_parse_header polycpp_forwarded_example_request_adapter
   ./build/examples/parse_header
   ./build/examples/request_adapter
