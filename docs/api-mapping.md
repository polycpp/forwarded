# API Mapping

| Upstream symbol | C++ symbol | Status | Notes |
|---|---|---|---|
| `forwarded(req)` | `polycpp::forwarded::forwarded(const RequestInfo&)` | adapted | C++ uses an explicit request adapter instead of duck-typed nested object reads. |
| internal `parse(header)` | `polycpp::forwarded::parse_header(std::string_view)` | compatibility layer | Public pure helper because it is useful without a request adapter and is central to behavior. |
| `req.headers['x-forwarded-for']` | `RequestInfo::headers` lookup | adapted | Header lookup is case-insensitive for C++ convenience while preserving Node lowercase behavior. |
| `req.socket.remoteAddress` | `RequestInfo::socket_remote_address` | adapted | Preferred over connection remote address, matching upstream 0.2.0. |
| `req.connection.remoteAddress` | `RequestInfo::connection_remote_address` | adapted | Fallback when socket remote address is absent. |
| `[socketAddr].concat(proxyAddrs)` | `forwarded(std::string_view, std::string_view)` | direct | Pure overload returns socket address first, then parsed proxy addresses. |
| missing `req` JavaScript `TypeError` | non-null C++ references plus `polycpp::TypeError` for missing remote address | adapted | C++ avoids nullable request APIs and validates adapter content. |
| upstream benchmark entry point | no public C++ symbol | deferred | Benchmark parity is outside v0 scope. |

## Framework object boundary review

- Upstream reads or mutates framework/request/response/context objects: yes, upstream reads a Node request-like object and does not mutate it.
- Upstream fields or methods read: `req.headers['x-forwarded-for']`, `req.socket.remoteAddress`, and `req.connection.remoteAddress`.
- Upstream fields or methods written: none.
- C++ adapter boundary: expose pure parser/address helpers plus `RequestInfo` for explicit request data; do not require duck-typed request objects.
- Partial mutation risk on validation failure: none, because all v0 APIs return values and do not mutate caller-owned request state.
