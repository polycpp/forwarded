# Test Plan

## Unit tests

- `parse_header` returns an empty vector for an empty header.
- `parse_header` returns entries in reverse `X-Forwarded-For` order.
- `parse_header` skips blank comma entries.
- `parse_header` trims leading and trailing ASCII space around entries.
- `parse_header` preserves non-space characters exactly, including tabs.
- `forwarded(remote_address, header)` prepends the socket address to parsed proxy addresses.
- `forwarded(RequestInfo)` uses socket remote address when present.
- `forwarded(RequestInfo)` falls back to connection remote address.
- `forwarded(RequestInfo)` prefers socket over connection when both are present.
- `forwarded(RequestInfo)` throws `polycpp::TypeError` when no remote address is available.

## Integration tests

- Build a `RequestInfo` with `X-Forwarded-For` and socket remote address and verify the same address order documented upstream.
- Build a `RequestInfo` with lowercase `x-forwarded-for` and with canonical `X-Forwarded-For` to verify case-insensitive header lookup.
- Compile and run example programs for pure parsing and request-adapter usage.

## Compatibility tests adapted from upstream

- `should require req`: adapt to missing remote address validation in `RequestInfo`.
- `should work with X-Forwarded-For header`: expect only the socket address when the header is absent.
- `should include entries from X-Forwarded-For`: adapt `10.0.0.2, 10.0.0.1` to return socket, `10.0.0.1`, `10.0.0.2`.
- `should skip blank entries`: adapt `10.0.0.2,, 10.0.0.1`.
- `should trim leading OWS`: adapt upstream space trimming case.
- socket address tests: begin with socket address, use `socket`, prefer `socket`, and fall back to `connection`.

## Release-blocking behaviors

- Address ordering must match upstream exactly.
- Blank entries must be skipped.
- ASCII space trimming must match upstream; tabs must not be silently converted to spaces.
- Socket remote address must take precedence over connection remote address.
- Public README examples must compile and match tests.
- Docs build must pass before public release.
