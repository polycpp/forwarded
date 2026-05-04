Installation
============

forwarded targets C++20 and builds with clang >= 14 or gcc >= 11. It depends on
the base polycpp library.

CMake FetchContent (recommended)
--------------------------------

Add the library to your ``CMakeLists.txt``:

.. code-block:: cmake

   include(FetchContent)

   FetchContent_Declare(
       polycpp_forwarded
       GIT_REPOSITORY https://github.com/polycpp/forwarded.git
       GIT_TAG        v1.0.0
   )
   FetchContent_MakeAvailable(polycpp_forwarded)

   add_executable(my_app main.cpp)
   target_link_libraries(my_app PRIVATE polycpp::forwarded)

The first configure pulls ``polycpp`` transitively, so the build tree may be
large. The snippet pins the 1.0.0 release; update ``GIT_TAG`` deliberately when
you upgrade.

Using a local clone
-------------------

If you already have forwarded and polycpp checked out side by side, tell
CMake to use them instead of fetching from GitHub:

.. code-block:: bash

   # Building this repo directly
   cmake -B build -G Ninja \
       -DFETCHCONTENT_SOURCE_DIR_POLYCPP=/path/to/polycpp

   # Consuming this repo through FetchContent
   cmake -B build -G Ninja \
       -DFETCHCONTENT_SOURCE_DIR_POLYCPP=/path/to/polycpp \
       -DFETCHCONTENT_SOURCE_DIR_POLYCPP_FORWARDED=/path/to/forwarded

This lets CMake use your existing checkouts instead of downloading dependencies.
The repository tests live under ``tests/``.

Build options
-------------

``POLYCPP_FORWARDED_BUILD_TESTS``
    Build the GoogleTest suite. Defaults to ``ON`` for standalone builds and
    ``OFF`` when consumed via FetchContent.

``POLYCPP_FORWARDED_BUILD_EXAMPLES``
    Build the runnable programs under ``examples/``. Defaults to ``OFF``.

``POLYCPP_IO``
    ``asio`` (default) or ``libuv`` - inherited from polycpp.

``POLYCPP_SSL_BACKEND``
    ``boringssl`` (default) or ``openssl``.

``POLYCPP_UNICODE``
    ``icu`` (recommended) or ``builtin``. ICU enables the Intl surface that
    several polycpp headers pull into their public signatures.

Verifying the install
---------------------

.. code-block:: bash

   cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ctest --test-dir build --output-on-failure

All tests should pass on a supported toolchain - if they do not, open an
issue on the `repository <https://github.com/polycpp/forwarded/issues>`_
with the compiler version and the failing test name.
