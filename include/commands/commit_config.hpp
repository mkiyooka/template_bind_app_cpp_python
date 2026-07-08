#pragma once

#include <string>

namespace commands {

struct CommitConfig {
    std::string message;
    bool amend = false;
    bool all = false;
};

} // namespace commands
