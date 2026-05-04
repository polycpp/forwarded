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

## Audit findings (libgen catch-up)

| ID | Severity | Location | Description | Recommended classification |
|---|---|---|---|---|
| AF-2026-05-04-A | low | `include/polycpp/forwarded/forwarded.hpp:28` | The catch-up audit found that public `HeaderMap` was a local `std::map<std::string, std::string>` even though current base polycpp provides `polycpp::http::Headers` for HTTP header semantics. | resolved 2026-05-04 - `HeaderMap` now aliases `polycpp::http::Headers`; pinned by `forwarded_request.uses_polycpp_http_headers_for_request_info` |
