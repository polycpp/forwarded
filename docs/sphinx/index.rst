forwarded
=========

**Parse X-Forwarded-For headers for polycpp.**

``polycpp::forwarded`` is a C++20 companion port of the npm ``forwarded``
package. It preserves upstream address ordering and adapts the Node.js request
object to explicit C++ request data.

.. code-block:: cpp

   #include <polycpp/forwarded/forwarded.hpp>

   auto addresses = polycpp::forwarded::forwarded(
       "127.0.0.1",
       "10.0.0.2, 10.0.0.1");

.. grid:: 2

   .. grid-item-card:: Upstream order
      :margin: 1

      Results start with the socket address, then forwarded entries from right
      to left.

   .. grid-item-card:: Pure parser
      :margin: 1

      Parse an ``X-Forwarded-For`` string without constructing a request object.

   .. grid-item-card:: Request adapter
      :margin: 1

      Use ``RequestInfo`` instead of duck-typed ``req.headers`` and nested
      socket properties.

   .. grid-item-card:: Compatibility tests
      :margin: 1

      The v0 tests are adapted from upstream ``test/test.js``.

Getting started
---------------

.. code-block:: cmake

   include(FetchContent)
   FetchContent_Declare(
       polycpp_forwarded
       GIT_REPOSITORY https://github.com/polycpp/forwarded.git
       GIT_TAG        master
   )
   FetchContent_MakeAvailable(polycpp_forwarded)
   target_link_libraries(my_app PRIVATE polycpp::forwarded)

:doc:`Installation <getting-started/installation>` | :doc:`Quickstart <getting-started/quickstart>` | :doc:`Tutorial <tutorials/parse-forwarded-for>` | :doc:`API reference <api/forwarded>`

.. toctree::
   :hidden:
   :caption: Getting started

   getting-started/installation
   getting-started/quickstart

.. toctree::
   :hidden:
   :caption: Tutorials

   tutorials/index

.. toctree::
   :hidden:
   :caption: How-to guides

   guides/index

.. toctree::
   :hidden:
   :caption: API reference

   api/index

.. toctree::
   :hidden:
   :caption: Examples

   examples/index
