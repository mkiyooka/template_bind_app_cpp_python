#pragma once

#include "commands/commit_config.hpp"
#include "commands/fetch_config.hpp"
#include "commands/log_config.hpp"

struct Config {
    commands::CommitConfig commit;
    commands::FetchConfig fetch;
    commands::LogConfig log;
};
