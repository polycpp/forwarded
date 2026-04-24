#include <iostream>

#include <polycpp/forwarded/forwarded.hpp>

int main() {
    const auto addresses = polycpp::forwarded::forwarded(
        "127.0.0.1",
        "10.0.0.2, 10.0.0.1");

    for (const auto& address : addresses) {
        std::cout << address << '\n';
    }
}
