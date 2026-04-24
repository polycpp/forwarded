# Dependency and JavaScript API Analysis

`scripts/analyze-upstream-js.py` was run after upstream intake. The JSON report is stored at `.tmp/dependency-analysis.json`.

- package: forwarded
- package version: 0.2.0
- package root: `/data/work/lib/forwarded/.tmp/upstream/forwarded`
- analyzer json: `.tmp/dependency-analysis.json`
- include dev dependencies: no
- dependency source install used: yes
- companion root checked: `/data/work/lib`

## Package entry metadata

- main: implicit `index.js`
- module: not declared
- types: none declared
- exports: none declared
- bin: none
- missing declared entries in repo clone: none
- TypeScript source files detected: 0

## Direct dependencies

- none detected

## Dependency ownership decisions

No direct runtime dependencies are declared. The target package source is owned by this repo under the upstream MIT license notice.

For every future dependency found in a later upstream version, choose exactly one outcome before coding:

- use existing polycpp companion
- create separate private polycpp companion repo
- implement private helper in this repo
- optional adapter
- deferred or unsupported feature

For every future dependency, also choose a license strategy before coding:

- permissive dependency ok with notice
- use existing companion license
- GPL-compatible repo or defer
- AGPL-compatible repo or defer
- optional GPL adapter or defer
- optional AGPL adapter or defer
- document LGPL obligations and linking model
- keep MPL code separated or replace/defer
- clean-room replacement
- dev/test-only, not shipped
- manual license review required

| Package | Kind | Requested | Installed | License | License evidence | License impact | License strategy | Affects repo license | Deps | Source files | Node API calls | JS API calls | Recommendation | Rationale |
|---|---|---|---|---|---|---|---|---|---:|---:|---:|---:|---|---|
| forwarded | target | 0.2.0 | 0.2.0 | MIT | package.json license field and upstream LICENSE file | permissive | permissive dependency ok with notice | no | 0 | 1 | 0 | 5 | port in this repo | Single-file runtime implementation with no runtime dependencies; license notice is straightforward. |

## License impact summary

- upstream package license: MIT
- repo license decision: keep this companion repo MIT and include upstream notice in `THIRD_PARTY_LICENSES.md`
- GPL/AGPL dependencies: none detected
- LGPL/MPL dependencies: none detected
- permissive dependencies requiring notices: upstream `forwarded` source notice only
- dev/test-only dependencies excluded from shipped artifacts: `beautify-benchmark`, `benchmark`, `deep-equal`, `eslint`, `mocha`, `nyc`, and related lint plugins
- dependency license notices to add to `THIRD_PARTY_LICENSES.md`: upstream `forwarded` MIT notice

## Transitive dependency summary

- none detected for runtime dependencies
- dev/test-only transitive dependencies are not shipped and are excluded from the port dependency graph

## Runtime API usage

### Target package

- entry points analyzed: `index.js`
- source files analyzed by analyzer: `index.js`
- source files manually inspected: `index.js`, `test/test.js`, `README.md`, `HISTORY.md`
- external imports seen from target: none in runtime implementation

### Node.js API usage

- none detected in runtime implementation; upstream public API still expects Node request-like properties, which are represented by a C++ `RequestInfo` adapter in this port

### JavaScript API usage

- analyzer-detected calls: `Array.prototype.push` (2), `String.prototype.substring` (2), `String.prototype.charCodeAt` (1)
- manually observed operations: backwards comma-list parsing, space-only trimming, skipped blank entries, and request property reads through `req.headers`, `req.socket.remoteAddress`, and `req.connection.remoteAddress`
- C++ replacements: `std::string_view`, `std::vector<std::string>`, `HeaderMap`, explicit socket/connection fields, and manual comma-list parsing

### Framework object API usage

- analyzer-reported framework object calls or mutations under libgen schema 3: none
- manual review decision: upstream reads request-like object fields; expose pure `parse_header(...)` and `forwarded(remote_address, header)` helpers plus an explicit `RequestInfo` adapter; do not require C++ callers to provide dynamic `req.headers` or nested `socket`/`connection` objects
- analyzer gap: schema 3 detects likely framework method calls and mutations but not framework object property reads; update `libgen` after this port

## Porting decisions

- Implement runtime behavior directly in C++ because there are no runtime dependencies and no Node APIs required for the core parser.
- Preserve the pure header parsing behavior and return order.
- Adapt `forwarded(req)` to `forwarded(RequestInfo)` because C++ should not rely on duck-typed request objects.
- Provide `forwarded(remote_address, header)` for callers that already have normalized request data.
- Preserve `req.socket` precedence over `req.connection` through `RequestInfo::socket_remote_address` before `RequestInfo::connection_remote_address`.
- Throw `polycpp::TypeError` when `forwarded(RequestInfo)` cannot determine a remote address.
- Keep parser whitespace behavior compatible with upstream by trimming ASCII space around entries and not treating tab as trim whitespace.

## Analyzer warnings

- none emitted by analyzer
