#pragma once

/**
 * @file detail/aggregator.hpp
 * @brief Inline implementation include point for polycpp/forwarded.
 * @since 0.1.0
 */

#include <polycpp/forwarded/forwarded.hpp>

#include <cstddef>
#include <string>
#include <string_view>

namespace polycpp::forwarded::detail {

inline char lower_ascii(char ch) noexcept {
    if (ch >= 'A' && ch <= 'Z') return static_cast<char>(ch - 'A' + 'a');
    return ch;
}

inline bool equals_ignore_ascii_case(std::string_view left, std::string_view right) noexcept {
    if (left.size() != right.size()) return false;
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (lower_ascii(left[i]) != lower_ascii(right[i])) return false;
    }
    return true;
}

inline HeaderMap::const_iterator find_header(const HeaderMap& headers, std::string_view name) {
    for (auto it = headers.begin(); it != headers.end(); ++it) {
        if (equals_ignore_ascii_case(it->first, name)) return it;
    }
    return headers.end();
}

inline std::string_view forwarded_header_value(const RequestInfo& request) {
    const auto found = find_header(request.headers, "x-forwarded-for");
    if (found == request.headers.end()) return {};
    return found->second;
}

inline std::string_view remote_address(const RequestInfo& request) {
    if (request.socket_remote_address) return *request.socket_remote_address;
    if (request.connection_remote_address) return *request.connection_remote_address;
    throw polycpp::TypeError("request remote address is required");
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

} // namespace polycpp::forwarded
