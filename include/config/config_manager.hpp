#pragma once

#include <string>
#include <vector>

#include <CLI/CLI.hpp>

#include "config/config_loader.hpp"

namespace config {

/**
 * @brief CLIオプションと設定ファイルを統合管理するクラス
 *
 * CLI11への全オプション登録と、設定ファイル読み込みを担い、
 * 優先度 (CLI引数 > 設定ファイル > デフォルト値) に従って最終的な Config を生成する。
 *
 * 使い方:
 * @code
 * ConfigManager mgr;
 * std::vector<std::string> config_paths;
 * app.add_option("-c,--config", config_paths, "Configuration file(s)");
 * auto *commit = app.add_subcommand("commit");
 * mgr.registerCommitOptions(*commit);   // commit スキーマ由来のオプションを登録
 * app.parse(argc, argv);                // CLI11がパース
 * Config conf = mgr.resolve(config_paths);  // 優先度解決
 * @endcode
 */
class ConfigManager {
public:
    ConfigManager();

    /// commit スキーマに基づいて CLI::App にオプションを登録する
    void registerCommitOptions(CLI::App &app);
    /// fetch スキーマに基づいて CLI::App にオプションを登録する
    void registerFetchOptions(CLI::App &app);
    /// log スキーマに基づいて CLI::App にオプションを登録する
    void registerLogOptions(CLI::App &app);

    /**
     * @brief スキーマ定義フィールドを優先度に従って解決して返す
     *
     * 優先度: CLI引数 > 設定ファイル > デフォルト値
     *
     * @param config_paths 設定ファイルパスのリスト (.conf マニフェストを含む)。空の場合はデフォルト探索
     * @return スキーマフィールドが解決済みの Config
     * @throws std::runtime_error 複数のデフォルト設定ファイルが存在する場合
     * @throws std::runtime_error 設定ファイルのパースに失敗した場合
     */
    Config resolve(const std::vector<std::string> &config_paths);

private:
    Config cli_values_;                      ///< CLI11のパース結果書き込み先
    std::vector<bool> commit_cli_set_;       ///< commit スキーマフィールドがCLIで明示指定されたか
    std::vector<bool> fetch_cli_set_;        ///< fetch スキーマフィールドがCLIで明示指定されたか
    std::vector<bool> log_cli_set_;          ///< log スキーマフィールドがCLIで明示指定されたか
    std::vector<std::string> log_paths_cli_; ///< log の paths (スキーマ外フィールド)
    bool log_paths_set_ = false;
};

} // namespace config
