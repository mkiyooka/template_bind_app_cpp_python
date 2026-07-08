#include "config_file_loader.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include <fkYAML/node.hpp>
#include <nlohmann/json.hpp>
#include <toml++/toml.hpp>

#include "config/config_schema.hpp"

namespace config {

namespace {

// ──────────────────────────────────────────────
// ドット区切りキー解決（共通テンプレート）
// ──────────────────────────────────────────────
//
// Accessor は以下の static メンバを持つ:
//   const Node* getChild(const Node& node, const std::string& key)
//   std::optional<T> getLeaf<T>(const Node& node, const std::string& key)

template <typename Accessor, typename T, typename Node>
std::optional<T> resolveDottedKey(const Node &root, std::string_view dotted_key) {
    const Node *current = &root;
    std::string_view remaining = dotted_key;

    while (true) {
        const auto dot_pos = remaining.find('.');
        if (dot_pos == std::string_view::npos) {
            return Accessor::template getLeaf<T>(*current, std::string(remaining));
        }
        const auto head = std::string(remaining.substr(0, dot_pos));
        remaining = remaining.substr(dot_pos + 1);
        current = Accessor::getChild(*current, head);
        if (current == nullptr) {
            return std::nullopt;
        }
    }
}

template <typename Accessor, typename Node, typename Owner, typename Schema>
void applySchema(const Node &root, Owner &owner, const Schema &schema) {
    std::apply(
        [&](auto &&...field) {
            (
                [&] {
                    using FieldType = std::remove_reference_t<decltype(owner.*field.member)>;
                    auto val = resolveDottedKey<Accessor, FieldType>(root, field.config_key);
                    if (val.has_value()) {
                        owner.*field.member = *val;
                    }
                }(),
                ...
            );
        },
        schema
    );
}

// ──────────────────────────────────────────────
// TOML アクセサ
// ──────────────────────────────────────────────

struct TomlAccessor {
    static const toml::table *getChild(const toml::table &node, const std::string &key) { return node[key].as_table(); }

    template <typename T>
    static std::optional<T> getLeaf(const toml::table &node, const std::string &key) {
        return node[key].template value<T>();
    }
};

void loadFromToml(const std::string &file_path, Config &conf) {
    const auto tbl = toml::parse_file(file_path);

    applySchema<TomlAccessor>(tbl, conf.commit, kCommitSchema);
    applySchema<TomlAccessor>(tbl, conf.fetch, kFetchSchema);
    applySchema<TomlAccessor>(tbl, conf.log, kLogSchema);

    if (const auto *log_tbl = tbl["log"].as_table()) {
        if (const auto *arr = (*log_tbl)["paths"].as_array()) {
            conf.log.paths.clear();
            for (const auto &el : *arr) {
                if (const auto val = el.value<std::string>()) {
                    conf.log.paths.push_back(*val);
                }
            }
        }
    }
}

// ──────────────────────────────────────────────
// JSON/JSONC アクセサ
// ──────────────────────────────────────────────

struct JsonAccessor {
    static const nlohmann::json *getChild(const nlohmann::json &node, const std::string &key) {
        if (!node.is_object() || !node.contains(key) || !node.at(key).is_object()) {
            return nullptr;
        }
        return &node.at(key);
    }

    template <typename T>
    static std::optional<T> getLeaf(const nlohmann::json &node, const std::string &key) {
        if (!node.is_object() || !node.contains(key)) {
            return std::nullopt;
        }
        return node.at(key).get<T>();
    }
};

void loadFromJson(const std::string &file_path, Config &conf) {
    std::ifstream ifs(file_path);
    if (!ifs) {
        throw std::runtime_error("Cannot open file: " + file_path);
    }
    const auto j = nlohmann::json::parse(
        ifs, /*callback cb=*/nullptr,
        /*allow_exceptions=*/true,
        /*ignore_comments=*/true
    );

    applySchema<JsonAccessor>(j, conf.commit, kCommitSchema);
    applySchema<JsonAccessor>(j, conf.fetch, kFetchSchema);
    applySchema<JsonAccessor>(j, conf.log, kLogSchema);

    if (j.contains("log") && j.at("log").is_object()) {
        const auto &log_obj = j.at("log");
        if (log_obj.contains("paths") && log_obj.at("paths").is_array()) {
            conf.log.paths.clear();
            for (const auto &el : log_obj.at("paths")) {
                if (el.is_string()) {
                    conf.log.paths.push_back(el.get<std::string>());
                }
            }
        }
    }
}

// ──────────────────────────────────────────────
// YAML アクセサ
// ──────────────────────────────────────────────

struct YamlAccessor {
    static const fkyaml::node *getChild(const fkyaml::node &node, const std::string &key) {
        if (!node.is_mapping() || !node.contains(key) || !node.at(key).is_mapping()) {
            return nullptr;
        }
        return &node.at(key);
    }

    template <typename T>
    static std::optional<T> getLeaf(const fkyaml::node &node, const std::string &key) {
        if (!node.is_mapping() || !node.contains(key)) {
            return std::nullopt;
        }
        return node.at(key).get_value<T>();
    }
};

void loadFromYaml(const std::string &file_path, Config &conf) {
    std::ifstream ifs(file_path);
    if (!ifs) {
        throw std::runtime_error("Cannot open file: " + file_path);
    }
    const auto root = fkyaml::node::deserialize(ifs);

    applySchema<YamlAccessor>(root, conf.commit, kCommitSchema);
    applySchema<YamlAccessor>(root, conf.fetch, kFetchSchema);
    applySchema<YamlAccessor>(root, conf.log, kLogSchema);

    const auto log_key = std::string("log");
    if (root.is_mapping() && root.contains(log_key)) {
        const auto &log_node = root.at(log_key);
        const auto paths_key = std::string("paths");
        if (log_node.is_mapping() && log_node.contains(paths_key) && log_node.at(paths_key).is_sequence()) {
            conf.log.paths.clear();
            for (const auto &el : log_node.at(paths_key)) {
                if (el.is_string()) {
                    conf.log.paths.push_back(el.get_value<std::string>());
                }
            }
        }
    }
}

} // namespace

// ──────────────────────────────────────────────
// 公開API
// ──────────────────────────────────────────────

void loadFromFile(const std::string &file_path, Config &conf) {
    const auto ext = std::filesystem::path(file_path).extension().string();
    if (ext == ".toml") {
        loadFromToml(file_path, conf);
    } else if (ext == ".json") {
        loadFromJson(file_path, conf);
    } else if (ext == ".yaml" || ext == ".yml") {
        loadFromYaml(file_path, conf);
    } else {
        throw std::runtime_error("Unsupported config file extension: " + ext);
    }
}

std::vector<std::string> expandManifest(const std::string &manifest_path) {
    std::ifstream ifs(manifest_path);
    if (!ifs) {
        throw std::runtime_error("Cannot open manifest file: " + manifest_path);
    }

    const auto parent_dir = std::filesystem::path(manifest_path).parent_path();
    std::vector<std::string> result;
    std::string line;

    while (std::getline(ifs, line)) {
        if (line.empty() || line.find_first_not_of(" \t") == std::string::npos) {
            continue;
        }
        if (line[line.find_first_not_of(" \t")] == '#') {
            continue;
        }

        auto path = std::filesystem::path(line);
        if (path.is_relative()) {
            path = parent_dir / path;
        }
        result.push_back(path.lexically_normal().string());
    }

    return result;
}

void loadFromFiles(const std::vector<std::string> &file_paths, Config &conf) {
    for (const auto &path : file_paths) {
        const auto ext = std::filesystem::path(path).extension().string();
        if (ext == ".conf") {
            for (const auto &expanded : expandManifest(path)) {
                loadFromFile(expanded, conf);
            }
        } else {
            loadFromFile(path, conf);
        }
    }
}

std::string findDefaultConfig() {
    const std::vector<std::string> candidates = {
        "config/default.toml",
        "config/default.json",
        "config/default.yaml",
    };

    std::vector<std::string> found;
    std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(found), [](const std::string &path) {
        return std::filesystem::exists(path);
    });

    if (found.size() > 1) {
        std::string msg = "Multiple default config files found:";
        for (const auto &p : found) {
            msg += " " + p;
        }
        throw std::runtime_error(msg);
    }

    return found.empty() ? std::string{} : found.front();
}

} // namespace config
