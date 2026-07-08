#include "commands/commit.hpp"

#include <iostream>
#include <stdexcept>

#include "commands/commit_config.hpp"

namespace commands {

void runCommit(const CommitConfig &config) {
    if (config.message.empty()) {
        throw std::invalid_argument("commit message must not be empty");
    }

    std::cout << "[commit] message=\"" << config.message << "\""
              << " amend=" << (config.amend ? "true" : "false") << " all=" << (config.all ? "true" : "false") << '\n';
}

} // namespace commands
