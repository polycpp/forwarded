#include <gtest/gtest.h>

#include <polycpp/forwarded/forwarded.hpp>
#include <polycpp/http/headers.hpp>

#include <type_traits>

namespace {

using polycpp::forwarded::AddressList;
using polycpp::forwarded::RequestInfo;

void expect_addresses(const AddressList& actual, std::initializer_list<std::string> expected) {
    EXPECT_EQ(actual, AddressList(expected));
}

} // namespace

TEST(parse_header, empty_header_returns_empty_list) {
    EXPECT_TRUE(polycpp::forwarded::parse_header("").empty());
}

TEST(parse_header, includes_entries_from_x_forwarded_for) {
    expect_addresses(
        polycpp::forwarded::parse_header("10.0.0.2, 10.0.0.1"),
        {"10.0.0.1", "10.0.0.2"});
}

TEST(parse_header, skips_blank_entries) {
    expect_addresses(
        polycpp::forwarded::parse_header("10.0.0.2,, 10.0.0.1"),
        {"10.0.0.1", "10.0.0.2"});
}

TEST(parse_header, trims_leading_and_trailing_spaces) {
    expect_addresses(
        polycpp::forwarded::parse_header(" 10.0.0.2 ,  , 10.0.0.1 "),
        {"10.0.0.1", "10.0.0.2"});
}

TEST(parse_header, only_trims_ascii_space) {
    expect_addresses(
        polycpp::forwarded::parse_header("\t10.0.0.2,\t10.0.0.1\t"),
        {"\t10.0.0.1\t", "\t10.0.0.2"});
}

TEST(forwarded, prepends_remote_address) {
    expect_addresses(
        polycpp::forwarded::forwarded("127.0.0.1", "10.0.0.2, 10.0.0.1"),
        {"127.0.0.1", "10.0.0.1", "10.0.0.2"});
}

TEST(forwarded_request, works_without_x_forwarded_for_header) {
    RequestInfo request;
    request.socket_remote_address = "127.0.0.1";

    expect_addresses(polycpp::forwarded::forwarded(request), {"127.0.0.1"});
}

TEST(forwarded_request, reads_x_forwarded_for_case_insensitively) {
    RequestInfo request;
    request.socket_remote_address = "127.0.0.1";
    request.headers.set("X-Forwarded-For", "10.0.0.2, 10.0.0.1");

    expect_addresses(
        polycpp::forwarded::forwarded(request),
        {"127.0.0.1", "10.0.0.1", "10.0.0.2"});
}

TEST(forwarded_request, uses_polycpp_http_headers_for_request_info) {
    static_assert(std::is_same_v<polycpp::forwarded::HeaderMap, polycpp::http::Headers>);

    RequestInfo request;
    request.socket_remote_address = "127.0.0.1";
    request.headers.set("x-forwarded-for", "10.0.0.2, 10.0.0.1");

    expect_addresses(
        polycpp::forwarded::forwarded(request),
        {"127.0.0.1", "10.0.0.1", "10.0.0.2"});
}

TEST(forwarded_request, prefers_socket_remote_address) {
    RequestInfo request;
    request.socket_remote_address = "127.0.0.1";
    request.connection_remote_address = "10.0.0.1";

    expect_addresses(polycpp::forwarded::forwarded(request), {"127.0.0.1"});
}

TEST(forwarded_request, falls_back_to_connection_remote_address) {
    RequestInfo request;
    request.connection_remote_address = "10.0.0.1";

    expect_addresses(polycpp::forwarded::forwarded(request), {"10.0.0.1"});
}

TEST(forwarded_request, requires_remote_address) {
    RequestInfo request;

    EXPECT_THROW(polycpp::forwarded::forwarded(request), polycpp::TypeError);
}

TEST(forwarded_http_request, uses_incoming_message_headers) {
    polycpp::http::IncomingMessage request;
    request.headers().set("X-Forwarded-For", "10.0.0.2, 10.0.0.1");

    expect_addresses(
        polycpp::forwarded::forwarded(request),
        {"", "10.0.0.1", "10.0.0.2"});
}

TEST(forwarded_http_request, returns_empty_remote_address_when_socket_is_absent) {
    polycpp::http::IncomingMessage request;

    expect_addresses(polycpp::forwarded::forwarded(request), {""});
}
