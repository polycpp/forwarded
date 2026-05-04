#pragma once

/**
 * @file forwarded.hpp
 * @brief Parse HTTP X-Forwarded-For header values.
 *
 * C++ port of npm forwarded (https://github.com/jshttp/forwarded). The
 * request-object API is adapted to explicit C++ request data.
 *
 * @see https://www.npmjs.com/package/forwarded
 * @since 0.1.0
 */

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <polycpp/core/error.hpp>
#include <polycpp/http.hpp>

namespace polycpp::forwarded {

/** @brief Ordered list of socket and forwarded addresses. */
using AddressList = std::vector<std::string>;

/** @brief HTTP headers used by the request adapter. */
using HeaderMap = polycpp::http::Headers;

/**
 * @brief Explicit C++ boundary for the upstream Node.js request object.
 *
 * Upstream reads the request header named `x-forwarded-for`,
 * `socket.remoteAddress`, and `connection.remoteAddress`.
 */
struct RequestInfo {
    /** @brief Request headers. `X-Forwarded-For` lookup is case-insensitive. */
    HeaderMap headers;

    /** @brief Preferred remote address, matching upstream `req.socket`. */
    std::optional<std::string> socket_remote_address;

    /** @brief Fallback remote address, matching upstream `req.connection`. */
    std::optional<std::string> connection_remote_address;
};

/**
 * @brief Parse `X-Forwarded-For` into proxy addresses.
 *
 * Results are returned in upstream order: rightmost header entry first.
 */
AddressList parse_header(std::string_view header);

/**
 * @brief Return remote address followed by parsed forwarded addresses.
 */
AddressList forwarded(std::string_view remote_address, std::string_view x_forwarded_for = {});

/**
 * @brief Adapt explicit request data to upstream `forwarded(req)` behavior.
 *
 * @throws polycpp::TypeError when no socket or connection remote address is present.
 */
AddressList forwarded(const RequestInfo& request);

}  // namespace polycpp::forwarded

#include <polycpp/forwarded/detail/aggregator.hpp>
