#include <iostream>

#include <polycpp/forwarded/forwarded.hpp>

int main() {
    polycpp::forwarded::RequestInfo request;
    request.socket_remote_address = "127.0.0.1";
    request.headers["X-Forwarded-For"] = "10.0.0.2, 10.0.0.1";

    const auto addresses = polycpp::forwarded::forwarded(request);
    for (const auto& address : addresses) {
        std::cout << address << '\n';
    }
}
