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

- upstream compatibility layout: upstream `test/test.js` is a single Mocha file; current C++ coverage is intentionally aggregated in `tests/test_smoke.cpp` because the upstream suite is small and all cases exercise the same parser/request adapter.
- upstream-to-local coverage map:
  - `test/test.js` `should require req` -> `forwarded_request.requires_remote_address`
  - `test/test.js` `should work with X-Forwarded-For header` -> `forwarded_request.works_without_x_forwarded_for_header`
  - `test/test.js` `should include entries from X-Forwarded-For` -> `parse_header.includes_entries_from_x_forwarded_for` and `forwarded.prepends_remote_address`
  - `test/test.js` `should skip blank entries` -> `parse_header.skips_blank_entries`
  - `test/test.js` `should trim leading OWS` -> `parse_header.trims_leading_and_trailing_spaces`
  - `test/test.js` socket-address cluster -> `forwarded_request.prefers_socket_remote_address` and `forwarded_request.falls_back_to_connection_remote_address`
- omitted upstream cases:
  - upstream HTTP server loopback fixture is not ported directly because `forwarded` does not export a server/listener API; `RequestInfo` integration tests cover the same observable address list.
  - upstream `deep-equal`/JSON response plumbing is JavaScript test harness behavior and is not meaningful for the typed C++ API.

## Security and fail-closed tests

- Missing remote address in `RequestInfo` must throw `polycpp::TypeError` instead of returning a misleading empty socket entry.
- Blank header entries must be skipped so malformed comma lists do not create empty addresses.
- Space-only trimming must stay explicit; tabs are preserved so the port does not silently normalize more than upstream.
- No trust-proxy policy is implemented; callers must treat parsed `X-Forwarded-For` values as untrusted unless they validate the proxy chain separately.

## Protocol/client tests

Not applicable because `forwarded` is a synchronous header parser, not a database, cache, queue, cloud-service, wire-protocol client, or listener/server package.

- service-backed e2e matrix: not applicable
- transport matrix, including TLS/compression decisions: not applicable
- auth or credential flow matrix: not applicable
- malformed packet / unsupported mode tests: not applicable
- binary payload type-mapping tests: not applicable
- stateful parser/session-state tests: not applicable
- server/listener response writer loopback tests: not applicable
- multi-result or unread-packet drain behavior: not applicable
- pool/session lifecycle tests: not applicable

## Release-blocking behaviors

- Address ordering must match upstream exactly.
- Blank entries must be skipped.
- ASCII space trimming must match upstream; tabs must not be silently converted to spaces.
- Socket remote address must take precedence over connection remote address.
- Public README examples must compile and match tests.
- Docs build must pass before public release.

## Current validation

Commands run during the 2026-05-04 libgen catch-up:

- `python3 /data/work/libgen/scripts/check-port-readiness.py --baseline /data/work/lib/forwarded` -> passed.
- `npm view forwarded version repository.url license dist.tarball --json` -> resolved latest npm version `0.2.0`.
- `python3 /data/work/libgen/scripts/intake-upstream.py /data/work/lib/forwarded https://github.com/jshttp/forwarded.git` -> fetched upstream, then refused to overwrite the completed research file without `--force`; no force overwrite was used.
- `python3 /data/work/libgen/scripts/intake-npm-package.py /data/work/lib/forwarded forwarded` -> extracted `forwarded@0.2.0` to `.tmp/npm-package`.
- `python3 /data/work/libgen/scripts/analyze-upstream-js.py /data/work/lib/forwarded /data/work/lib/forwarded/.tmp/npm-package` -> wrote `.tmp/dependency-analysis.json`; analyzer reported 4 target framework-object hits, 0 dependency hits, and no warnings.
- `python3 /data/work/libgen/scripts/check-port-readiness.py --strict /data/work/lib/forwarded` -> passed.
- `cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DFETCHCONTENT_SOURCE_DIR_POLYCPP=/data/work/gitlab-workspace/polycpp -DPOLYCPP_FORWARDED_BUILD_EXAMPLES=ON` -> configured with base polycpp snapshot `75bc07dfca6ac0aaca07c8748476246e8c18df74`.
- `cmake --build build -j$(nproc)` -> built `polycpp_forwarded`, `polycpp_forwarded_test_smoke`, and both example targets.
- `ctest --test-dir build --output-on-failure` -> 11/11 tests passed.
- `./build/examples/parse_header` -> printed `127.0.0.1`, `10.0.0.1`, `10.0.0.2`.
- `./build/examples/request_adapter` -> printed `127.0.0.1`, `10.0.0.1`, `10.0.0.2`.
- `python3 docs/build.py` -> Doxygen/Sphinx build passed with warnings treated as errors.
- `python3 /data/work/libgen/scripts/check-port-validation.py --run-docs-build /data/work/lib/forwarded` -> post-implementation validation passed, including the docs build.
