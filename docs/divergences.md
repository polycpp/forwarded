# Divergences From Upstream

## Deferred Features

- Duck-typed Node.js `IncomingMessage` integration is deferred; use `RequestInfo` or pure overloads instead.
- Live `polycpp::http` request integration is deferred until a stable shared request adapter is chosen.
- Upstream benchmark harness parity is deferred.
- Exact JavaScript property-access failure behavior for malformed nested request objects is deferred.

## Deliberate Behavior Changes

- `forwarded(req)` becomes `forwarded(const RequestInfo&)`, making request data explicit and typed.
- Header lookup in `RequestInfo::headers` is case-insensitive so C++ callers do not need to pre-normalize header names to lowercase.
- Missing remote address in `RequestInfo` throws `polycpp::TypeError`; missing request objects are not representable because the C++ API uses references.

## Unsupported Runtime-Specific Features

- Dynamic Node request objects with arbitrary `headers`, `socket`, and `connection` property shapes are not supported.
- Node.js HTTP server integration is not part of v0.
