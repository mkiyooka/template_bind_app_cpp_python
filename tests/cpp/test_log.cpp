#include <exception>
#include <iostream>
#include <stdexcept>

#include "commands/log.hpp"
#include "commands/log_config.hpp"

namespace {

bool testRunLogSucceedsWithPaths() {
    commands::LogConfig config;
    config.max_count = 5;
    config.paths = {"src", "tests"};

    try {
        commands::runLog(config);
    } catch (const std::exception &e) {
        std::cout << "FAIL: unexpected exception: " << e.what() << '\n';
        return false;
    }
    return true;
}

bool testRunLogRejectsNonPositiveMaxCount() {
    commands::LogConfig config;
    config.max_count = 0;

    try {
        commands::runLog(config);
    } catch (const std::invalid_argument &) {
        return true;
    }
    std::cout << "FAIL: expected std::invalid_argument for non-positive max_count\n";
    return false;
}

} // namespace

int main() noexcept {
    try {
        if (!testRunLogSucceedsWithPaths()) {
            return 1;
        }
        if (!testRunLogRejectsNonPositiveMaxCount()) {
            return 1;
        }
    } catch (...) {
        std::cout << "EXCEPTION: Unexpected error occurred\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}
