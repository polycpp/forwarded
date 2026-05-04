# polycpp-forwarded

C++ companion port of [forwarded](https://www.npmjs.com/package/forwarded) for the [polycpp](https://github.com/polycpp) project.

`forwarded` parses the HTTP `X-Forwarded-For` header and returns the socket address followed by forwarded addresses in the same order as the upstream npm package.

`X-Forwarded-For` is client-spoofable unless trusted infrastructure sets or overwrites it. This library parses address lists only; callers must decide which proxy entries are trusted before using returned addresses for authentication, rate limiting, audit logging, or access control.

## Status

Port version: `1.0.0`

Initial port based on upstream version: `0.2.0`

Compatibility note:

- This repo does not imply full parity with upstream `forwarded`.
- Implemented and deferred behavior is tracked in `docs/research.md`, `docs/api-mapping.md`, and `docs/divergences.md`.

Implemented:

- `parse_header` for pure `X-Forwarded-For` parsing.
- Pure address construction with `forwarded(remote_address, header)`.
- Explicit `RequestInfo` adapter for request-like data.
- Live `polycpp::http::IncomingMessage` adapter for HTTP server handlers.
- Compatibility behavior for blank entries, space trimming, and socket precedence.

Deferred:

- Duck-typed Node.js `IncomingMessage` integration.
- Upstream benchmark harness parity.

Known divergences:

- The C++ API uses typed request data instead of dynamic `req.headers`, `req.socket`, and `req.connection` property reads.
- `forwarded(RequestInfo)` throws `polycpp::TypeError` when both remote address fields are absent; the live `forwarded(polycpp::http::IncomingMessage)` overload returns an empty first address when no socket or connection remote address is available.

## Prerequisites

- C++20 compiler
- CMake 3.20+
- Ninja recommended

## Build

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
cd build && ctest --output-on-failure
```

## Usage

```cpp
#include <polycpp/forwarded/forwarded.hpp>

polycpp::forwarded::RequestInfo req;
req.socket_remote_address = "127.0.0.1";
req.headers.set("X-Forwarded-For", "10.0.0.2, 10.0.0.1");

polycpp::forwarded::AddressList addresses = polycpp::forwarded::forwarded(req);
// addresses == {"127.0.0.1", "10.0.0.1", "10.0.0.2"}
```

## License

MIT
