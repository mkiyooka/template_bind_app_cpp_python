#pragma once

#include <string>
#include <vector>

namespace commands {

struct LogConfig {
    int max_count = 10;
    std::vector<std::string> paths;
};

} // namespace commands
