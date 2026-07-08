#include <iostream>
#include <string>

#include "commands/commit.hpp"
#include "commands/commit_config.hpp"

/**
 * Main debug program for testing command implementations directly (without Python).
 */

static void testCommitCommand() {
    std::cout << "\n--- Testing commit command ---" << "\n";

    commands::CommitConfig config;
    config.message = "debug commit";
    config.amend = false;
    config.all = true;

    commands::runCommit(config);
}

int main(int argc, char *argv[]) {
    std::cout << "=== Command Debug Program ===" << "\n";

    if (argc > 1) {
        std::string arg = std::string(argv[1]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (arg == "--commit") {
            testCommitCommand();
            return 0;
        }
        if (arg == "--help") {
            std::string program_name = std::string(argv[0]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            std::cout << "Usage: " << program_name << " [--commit|--help]" << "\n";
            std::cout << "  --commit  Test commit command only" << "\n";
            std::cout << "  --help    Show this help message" << "\n";
            return 0;
        }
    }

    testCommitCommand();

    std::cout << "\n=== Debug Complete ===" << "\n";
    return 0;
}
