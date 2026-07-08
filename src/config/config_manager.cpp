#include <cstddef>
#include <string>
#include <tuple>
#include <type_traits>

#include "config/config_manager.hpp"
#include "config/config_schema.hpp"
#include "config_file_loader.hpp"

namespace config {

namespace {

constexpr std::size_t kCommitSchemaSize = std::tuple_size_v<decltype(kCommitSchema)>;
constexpr std::size_t kFetchSchemaSize = std::tuple_size_v<decltype(kFetchSchema)>;
constexpr std::size_t kLogSchemaSize = std::tuple_size_v<decltype(kLogSchema)>;

template <typename Owner, typename Schema>
void registerOptions(CLI::App &app, Owner &cli_values, std::vector<bool> &cli_set, const Schema &schema) {
    std::size_t idx = 0;
    std::apply(
        [&](auto &&...field) {
            (
                [&] {
                    const std::size_t i = idx++;
                    using FieldType = std::remove_reference_t<decltype(cli_values.*field.member)>;
                    if constexpr (std::is_same_v<FieldType, bool>) {
                        auto *opt = app.add_flag(
                            std::string(field.cli_option), cli_values.*field.member, std::string(field.description)
                        );
                        opt->each([&cli_set, i](const std::string & /*unused*/) { cli_set[i] = true; });
                    } else {
                        auto *opt = app.add_option(
                            std::string(field.cli_option), cli_values.*field.member, std::string(field.description)
                        );
                        opt->each([&cli_set, i](const std::string & /*unused*/) { cli_set[i] = true; });
                    }
                }(),
                ...
            );
        },
        schema
    );
}

template <typename Owner, typename Schema>
void resolveSchema(
    Owner &result, const Owner &file_values, const Owner &cli_values, const std::vector<bool> &cli_set,
    const Schema &schema
) {
    std::apply([&](auto &&...field) { ((result.*field.member = file_values.*field.member), ...); }, schema);

    std::size_t idx = 0;
    std::apply(
        [&](auto &&...field) {
            (
                [&] {
                    if (cli_set[idx++]) {
                        result.*field.member = cli_values.*field.member;
                    }
                }(),
                ...
            );
        },
        schema
    );
}

} // namespace

ConfigManager::ConfigManager()
    : cli_values_{},
      commit_cli_set_(kCommitSchemaSize, false),
      fetch_cli_set_(kFetchSchemaSize, false),
      log_cli_set_(kLogSchemaSize, false) {}

void ConfigManager::registerCommitOptions(CLI::App &app) {
    registerOptions(app, cli_values_.commit, commit_cli_set_, kCommitSchema);
}

void ConfigManager::registerFetchOptions(CLI::App &app) {
    registerOptions(app, cli_values_.fetch, fetch_cli_set_, kFetchSchema);
}

void ConfigManager::registerLogOptions(CLI::App &app) {
    registerOptions(app, cli_values_.log, log_cli_set_, kLogSchema);
    auto *opt = app.add_option("--path", log_paths_cli_, "Restrict output to the given path(s)");
    opt->each([this](const std::string & /*unused*/) { log_paths_set_ = true; });
}

Config ConfigManager::resolve(const std::vector<std::string> &config_paths) {
    Config result{};

    Config file_values{};
    std::vector<std::string> paths = config_paths;
    if (paths.empty()) {
        const std::string default_path = findDefaultConfig();
        if (!default_path.empty()) {
            paths.push_back(default_path);
        }
    }
    if (!paths.empty()) {
        loadFromFiles(paths, file_values);
    }

    resolveSchema(result.commit, file_values.commit, cli_values_.commit, commit_cli_set_, kCommitSchema);
    resolveSchema(result.fetch, file_values.fetch, cli_values_.fetch, fetch_cli_set_, kFetchSchema);
    resolveSchema(result.log, file_values.log, cli_values_.log, log_cli_set_, kLogSchema);

    result.log.paths = log_paths_set_ ? log_paths_cli_ : file_values.log.paths;

    return result;
}

} // namespace config
