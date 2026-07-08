#include "app_cli/cli.hpp"

#include <cstdlib>
#include <exception>
#include <vector>

#include <CLI/CLI.hpp>
#include <fmt/base.h>

#include "commands/commit.hpp"
#include "commands/fetch.hpp"
#include "commands/log.hpp"
#include "config/config_manager.hpp"

int runCli(int argc, char *argv[]) {
    CLI::App app{"Command Binding Template CLI"};
    argv = app.ensure_utf8(argv);

    std::vector<std::string> config_files;
    app.add_option("-c,--config", config_files, "Configuration file(s)");

    config::ConfigManager config_manager;

    auto *commit = app.add_subcommand("commit", "Record changes to the repository");
    config_manager.registerCommitOptions(*commit);

    auto *fetch = app.add_subcommand("fetch", "Download objects and refs from another repository");
    config_manager.registerFetchOptions(*fetch);

    auto *log = app.add_subcommand("log", "Show commit logs");
    config_manager.registerLogOptions(*log);

    app.require_subcommand(1);

    try {
        app.parse(argc, argv);
    } catch (const CLI::CallForHelp &e) {
        std::exit(app.exit(e));
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    const Config conf = config_manager.resolve(config_files);

    try {
        if (app.got_subcommand("commit")) {
            commands::runCommit(conf.commit);
        } else if (app.got_subcommand("fetch")) {
            commands::runFetch(conf.fetch);
        } else if (app.got_subcommand("log")) {
            commands::runLog(conf.log);
        }
    } catch (const std::exception &e) {
        fmt::print(stderr, "Error: {}\n", e.what());
        return 1;
    }

    return 0;
}
