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

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <polycpp/core/error.hpp>

namespace polycpp::forwarded {

using AddressList = std::vector<std::string>;
using HeaderMap = std::map<std::string, std::string>;

/**
 * @brief Explicit C++ boundary for the upstream Node.js request object.
 *
 * Upstream reads `req.headers['x-forwarded-for']`,
 * `req.socket.remoteAddress`, and `req.connection.remoteAddress`.
 */
struct RequestInfo {
    HeaderMap headers;
    std::optional<std::string> socket_remote_address;
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
