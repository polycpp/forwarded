# polycpp-forwarded

C++ companion port of [forwarded](https://www.npmjs.com/package/forwarded) for [polycpp](https://github.com/enricohuang/polycpp).

`forwarded` parses the HTTP `X-Forwarded-For` header and returns the socket address followed by forwarded addresses in the same order as the upstream npm package.

## Status

Port version: `0.1.0`

Initial port based on upstream version: `0.2.0`

Compatibility note:

- This repo does not imply full parity with upstream `forwarded`.
- Implemented and deferred behavior is tracked in `docs/research.md`, `docs/api-mapping.md`, and `docs/divergences.md`.

Implemented:

- Planned v0 API: `parse_header`, pure address construction, and explicit `RequestInfo` adapter.
- Planning docs are complete enough for implementation to start after strict readiness passes.

Deferred:

- Duck-typed Node.js `IncomingMessage` integration.
- Live `polycpp::http` request integration.
- Upstream benchmark harness parity.

Known divergences:

- The C++ API uses typed request data instead of dynamic `req.headers`, `req.socket`, and `req.connection` property reads.

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
req.headers["X-Forwarded-For"] = "10.0.0.2, 10.0.0.1";

polycpp::forwarded::AddressList addresses = polycpp::forwarded::forwarded(req);
// addresses == {"127.0.0.1", "10.0.0.1", "10.0.0.2"}
```

## License

MIT
