#pragma once

#include <string_view>
#include <tuple>

#include "commands/commit_config.hpp"
#include "commands/fetch_config.hpp"
#include "commands/log_config.hpp"

namespace config {

/**
 * @brief Config構造体のフィールドに対するCLIオプションと設定ファイルキーのマッピング記述子
 *
 * @tparam Owner フィールドを持つ構造体型
 * @tparam T フィールドの型
 */
template <typename Owner, typename T>
struct FieldDescriptor {
    std::string_view cli_option;  ///< CLI11オプション名 (例: "--message")
    std::string_view config_key;  ///< 設定ファイルキー (例: "commit.message", ドット区切りでネスト表現)
    std::string_view description; ///< CLIヘルプ文字列
    T Owner::*member;             ///< ポインタ・トゥ・メンバー
};

// CTAD補助 (C++17): FieldDescriptor<Owner, T>(...) の型推論を有効にする
template <typename Owner, typename T>
FieldDescriptor(std::string_view, std::string_view, std::string_view, T Owner::*) -> FieldDescriptor<Owner, T>;

/**
 * @brief commit コマンドのスキーマ定義
 */
inline constexpr auto kCommitSchema = std::make_tuple(
    FieldDescriptor{"--message", "commit.message", "Commit message", &commands::CommitConfig::message},
    FieldDescriptor{"--amend", "commit.amend", "Amend the previous commit", &commands::CommitConfig::amend},
    FieldDescriptor{"--all", "commit.all", "Stage all modified files", &commands::CommitConfig::all}
);

/**
 * @brief fetch コマンドのスキーマ定義
 */
inline constexpr auto kFetchSchema = std::make_tuple(
    FieldDescriptor{"--remote", "fetch.remote", "Remote name to fetch from", &commands::FetchConfig::remote},
    FieldDescriptor{
        "--prune", "fetch.prune", "Remove remote-tracking branches that no longer exist", &commands::FetchConfig::prune
    }
);

/**
 * @brief log コマンドのスキーマ定義
 *
 * paths (std::vector<std::string>) はスキーマ管理の対象外で、
 * config_file_loader.cpp / CLI11 で個別に読み込む。
 */
inline constexpr auto kLogSchema = std::make_tuple(
    FieldDescriptor{
        "--max-count", "log.max_count", "Maximum number of commits to show", &commands::LogConfig::max_count
    }
);

} // namespace config
