forwarded
=========

**Parse X-Forwarded-For headers for polycpp.**

``polycpp::forwarded`` is a C++20 companion port of the npm ``forwarded``
package. It preserves upstream address ordering and adapts the Node.js request
object to explicit C++ request data.

.. warning::

   ``X-Forwarded-For`` is client-spoofable unless it is set or overwritten by
   trusted infrastructure. This library parses address lists; it does not decide
   which proxies are trusted. Choose and enforce that policy before using
   returned addresses for authentication, rate limiting, audit logging, or
   access control.

.. code-block:: cpp

   #include <polycpp/forwarded/forwarded.hpp>

   auto addresses = polycpp::forwarded::forwarded(
       "127.0.0.1",
       "10.0.0.2, 10.0.0.1");

.. grid:: 2

   .. grid-item-card:: Install
      :margin: 1
      :link: getting-started/installation
      :link-type: doc

      Add the 1.0.0 release with CMake FetchContent.

   .. grid-item-card:: Quickstart
      :margin: 1
      :link: getting-started/quickstart
      :link-type: doc

      Build the same address list from explicit values and request data.

   .. grid-item-card:: Tutorial
      :margin: 1
      :link: tutorials/parse-forwarded-for
      :link-type: doc

      Parse headers, adapt ``RequestInfo``, and call the live HTTP overload.

   .. grid-item-card:: Adapter guide
      :margin: 1
      :link: guides/request-adapter-boundary
      :link-type: doc

      Map Node-style request fields to explicit C++ request boundaries.

   .. grid-item-card:: API reference
      :margin: 1
      :link: api/forwarded
      :link-type: doc

      Review overload behavior, return ordering, and missing remote handling.

   .. grid-item-card:: Examples
      :margin: 1
      :link: examples/index
      :link-type: doc

      Run the small programs under ``examples/``.

Getting started
---------------

.. code-block:: cmake

   include(FetchContent)
   FetchContent_Declare(
       polycpp_forwarded
       GIT_REPOSITORY https://github.com/polycpp/forwarded.git
       GIT_TAG        v1.0.0
   )
   FetchContent_MakeAvailable(polycpp_forwarded)
   target_link_libraries(my_app PRIVATE polycpp::forwarded)

:doc:`Installation <getting-started/installation>` | :doc:`Quickstart <getting-started/quickstart>` | :doc:`Tutorial <tutorials/parse-forwarded-for>` | :doc:`Adapter guide <guides/request-adapter-boundary>` | :doc:`API reference <api/forwarded>` | :doc:`Examples <examples/index>`

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
