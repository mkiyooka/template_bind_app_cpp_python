#pragma once

#include <string>

namespace commands {

struct FetchConfig {
    std::string remote = "origin";
    bool prune = false;
};

} // namespace commands
