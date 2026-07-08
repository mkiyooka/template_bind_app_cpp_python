#include "commands/log.hpp"

#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "commands/log_config.hpp"

namespace commands {

void runLog(const LogConfig &config) {
    if (config.max_count <= 0) {
        throw std::invalid_argument("log max_count must be positive");
    }

    std::cout << "[log] max_count=" << config.max_count << " paths=[";
    for (size_t i = 0; i < config.paths.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << config.paths[i];
    }
    std::cout << "]\n";
}

} // namespace commands
