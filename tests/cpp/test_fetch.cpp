#include <exception>
#include <iostream>
#include <stdexcept>

#include "commands/fetch.hpp"
#include "commands/fetch_config.hpp"

namespace {

bool testRunFetchSucceedsWithDefaultRemote() {
    const commands::FetchConfig config;

    try {
        commands::runFetch(config);
    } catch (const std::exception &e) {
        std::cout << "FAIL: unexpected exception: " << e.what() << '\n';
        return false;
    }
    return true;
}

bool testRunFetchRejectsEmptyRemote() {
    commands::FetchConfig config;
    config.remote = "";

    try {
        commands::runFetch(config);
    } catch (const std::invalid_argument &) {
        return true;
    }
    std::cout << "FAIL: expected std::invalid_argument for empty remote\n";
    return false;
}

} // namespace

int main() noexcept {
    try {
        if (!testRunFetchSucceedsWithDefaultRemote()) {
            return 1;
        }
        if (!testRunFetchRejectsEmptyRemote()) {
            return 1;
        }
    } catch (...) {
        std::cout << "EXCEPTION: Unexpected error occurred\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}
