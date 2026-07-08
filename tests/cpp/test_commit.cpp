#include <exception>
#include <iostream>
#include <stdexcept>

#include "commands/commit.hpp"
#include "commands/commit_config.hpp"

namespace {

bool testRunCommitSucceedsWithMessage() {
    commands::CommitConfig config;
    config.message = "test commit";

    try {
        commands::runCommit(config);
    } catch (const std::exception &e) {
        std::cout << "FAIL: unexpected exception: " << e.what() << '\n';
        return false;
    }
    return true;
}

bool testRunCommitRejectsEmptyMessage() {
    commands::CommitConfig config;
    config.message = "";

    try {
        commands::runCommit(config);
    } catch (const std::invalid_argument &) {
        return true;
    }
    std::cout << "FAIL: expected std::invalid_argument for empty message\n";
    return false;
}

} // namespace

int main() noexcept {
    try {
        if (!testRunCommitSucceedsWithMessage()) {
            return 1;
        }
        if (!testRunCommitRejectsEmptyMessage()) {
            return 1;
        }
    } catch (...) {
        std::cout << "EXCEPTION: Unexpected error occurred\n";
        return 1;
    }
    std::cout << "PASS\n";
    return 0;
}
