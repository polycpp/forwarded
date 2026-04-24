Examples
========

Runnable programs under ``examples/`` use only the public API.

.. toctree::
   :maxdepth: 1

   parse-header
   request-adapter

Running examples
----------------

From the repository root:

.. code-block:: bash

   cmake -B build -G Ninja -DPOLYCPP_FORWARDED_BUILD_EXAMPLES=ON
   cmake --build build --target parse_header request_adapter
   ./build/examples/parse_header
   ./build/examples/request_adapter
