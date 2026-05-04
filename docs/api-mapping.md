# API Mapping

| Upstream symbol | C++ symbol | Status | Notes |
|---|---|---|---|
| `forwarded(req)` | `polycpp::forwarded::forwarded(const RequestInfo&)` | adapted | C++ uses an explicit request adapter instead of duck-typed nested object reads. |
| internal `parse(header)` | `polycpp::forwarded::parse_header(std::string_view)` | compatibility layer | Public pure helper because it is useful without a request adapter and is central to behavior. |
| `req.headers['x-forwarded-for']` | `RequestInfo::headers` (`polycpp::http::Headers`) lookup | adapted | Header lookup uses the shared polycpp HTTP header container. |
| `req.socket.remoteAddress` | `RequestInfo::socket_remote_address` | adapted | Preferred over connection remote address, matching upstream 0.2.0. |
| `req.connection.remoteAddress` | `RequestInfo::connection_remote_address` | adapted | Fallback when socket remote address is absent. |
| live `polycpp::http::IncomingMessage` request | `polycpp::forwarded::forwarded(const polycpp::http::IncomingMessage&)` | compatibility layer | Reads `headers()` and `socket()` from a live polycpp HTTP request; missing socket or remote address maps to an empty first address. |
| `[socketAddr].concat(proxyAddrs)` | `forwarded(std::string_view, std::string_view)` | direct | Pure overload returns socket address first, then parsed proxy addresses. |
| missing `req` JavaScript `TypeError` | non-null C++ references plus `polycpp::TypeError` for missing remote address | adapted | C++ avoids nullable request APIs and validates adapter content. |
| upstream benchmark entry point | no public C++ symbol | deferred | Benchmark parity is outside v0 scope. |

## TypeScript Declaration Review

- Declaration source used: none; upstream `forwarded@0.2.0` does not ship TypeScript declarations in the repo clone or published npm artifact.
- Public APIs, overloads, options, callbacks, streams, or literal unions found only or most clearly in declarations: none.
- Declaration-only globals, caches, deprecated fields, or runtime-specific surfaces mapped as unsupported/not-applicable: none.

## Framework object boundary review

- Upstream reads or mutates framework/request/response/context objects: yes, upstream reads a Node request-like object and does not mutate it.
- Upstream fields or methods read: `req.headers['x-forwarded-for']`, `req.socket.remoteAddress`, and `req.connection.remoteAddress`.
- Upstream fields or methods written: none.
- C++ adapter boundary: expose pure parser/address helpers, `RequestInfo` for explicit request data, and a live `polycpp::http::IncomingMessage` overload. Do not require duck-typed request objects. `HeaderMap` is an alias for `polycpp::http::Headers`, so header lookup and validation use the shared HTTP header container.
- Partial mutation risk on validation failure: none, because all v0 APIs return values and do not mutate caller-owned request state.

## Node parity surface review

- Callback APIs: none in the public runtime API.
- Promise APIs: none.
- EventEmitter APIs: none; upstream test response events are test harness behavior only.
- Server/listener APIs: none exported; TCP, Unix/IPC path, adopted native handles, HTTP/HTTPS, TLS, and generic stream listener primitives exist in base polycpp but are not selected because `forwarded` owns no listener lifecycle.
- Diagnostic/tracing APIs: none.
- Stream APIs: none; upstream stream use is limited to the HTTP test fixture.
- Buffer and binary APIs: none.
- URL, timer, process, and filesystem APIs: none.
- Crypto, compression, TLS, network, and HTTP APIs: no runtime Node built-ins; HTTP request field reads are adapted to explicit values and to the live `polycpp::http::IncomingMessage` overload.
- Unsupported or non-meaningful Node-specific APIs and audit reason: CommonJS packaging, arbitrary duck-typed request object shapes, and exact JavaScript property-access failure modes are not represented by the typed C++ API.
