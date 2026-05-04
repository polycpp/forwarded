#pragma once

/**
 * @file detail/aggregator.hpp
 * @brief Inline implementation include point for polycpp/forwarded.
 * @since 1.0.0
 */

#include <polycpp/forwarded/forwarded.hpp>

#include <cstddef>
#include <string>
#include <string_view>

namespace polycpp::forwarded::detail {

inline std::string forwarded_header_value(const RequestInfo& request) {
    return request.headers.get("x-forwarded-for").value_or("");
}

inline std::string_view remote_address(const RequestInfo& request) {
    if (request.socket_remote_address) return *request.socket_remote_address;
    if (request.connection_remote_address) return *request.connection_remote_address;
    throw polycpp::TypeError("request remote address is required");
}

inline std::string remote_address(const polycpp::http::IncomingMessage& request) {
    if (const auto socket = request.socket()) {
        return socket->remoteAddress().value_or("");
    }
    if (const auto connection = request.connection()) {
        return connection->remoteAddress().value_or("");
    }
    return "";
}

} // namespace polycpp::forwarded::detail

namespace polycpp::forwarded {

inline AddressList parse_header(std::string_view header) {
    std::size_t end = header.size();
    AddressList list;
    std::size_t start = header.size();

    for (std::size_t i = header.size(); i > 0; --i) {
        const std::size_t index = i - 1;
        switch (static_cast<unsigned char>(header[index])) {
        case 0x20:
            if (start == end) start = end = index;
            break;
        case 0x2c:
            if (start != end) list.emplace_back(header.substr(start, end - start));
            start = end = index;
            break;
        default:
            start = index;
            break;
        }
    }

    if (start != end) list.emplace_back(header.substr(start, end - start));
    return list;
}

inline AddressList forwarded(std::string_view remote_address, std::string_view x_forwarded_for) {
    AddressList addrs;
    addrs.emplace_back(remote_address);

    AddressList proxy_addrs = parse_header(x_forwarded_for);
    addrs.insert(addrs.end(), proxy_addrs.begin(), proxy_addrs.end());
    return addrs;
}

inline AddressList forwarded(const RequestInfo& request) {
    return forwarded(detail::remote_address(request), detail::forwarded_header_value(request));
}

inline AddressList forwarded(const polycpp::http::IncomingMessage& request) {
    return forwarded(
        detail::remote_address(request),
        request.headers().get("x-forwarded-for").value_or(""));
}

} // namespace polycpp::forwarded
