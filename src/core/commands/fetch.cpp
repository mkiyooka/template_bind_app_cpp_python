#include "commands/fetch.hpp"

#include <iostream>
#include <stdexcept>

#include "commands/fetch_config.hpp"

namespace commands {

void runFetch(const FetchConfig &config) {
    if (config.remote.empty()) {
        throw std::invalid_argument("fetch remote must not be empty");
    }

    std::cout << "[fetch] remote=\"" << config.remote << "\""
              << " prune=" << (config.prune ? "true" : "false") << '\n';
}

} // namespace commands
