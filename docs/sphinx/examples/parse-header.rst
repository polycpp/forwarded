Remote/Header Example
=====================

This program calls ``forwarded(remote_address, header)``. It prints the supplied
remote address first, followed by parsed ``X-Forwarded-For`` entries from right
to left. Use ``parse_header`` directly only when you want the header entries
without a remote address.

Source
------

.. literalinclude:: ../../../examples/parse_header.cpp
   :language: cpp

Output
------

.. code-block:: text

   127.0.0.1
   10.0.0.1
   10.0.0.2
