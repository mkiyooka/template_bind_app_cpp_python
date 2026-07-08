#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "commands/commit.hpp"
#include "commands/commit_config.hpp"
#include "commands/fetch.hpp"
#include "commands/fetch_config.hpp"
#include "commands/log.hpp"
#include "commands/log_config.hpp"

namespace nb = nanobind;
using namespace nb::literals;

NB_MODULE(_nanobind_module, m) {
    m.doc() = R"nbdoc(
        Command Binding Template

        Pythonのtyper CLIが組み立てた設定(Config)をC++のコマンド実装に渡すためのバインディングです。
    )nbdoc";

    nb::class_<commands::CommitConfig>(
        m, "CommitConfig",
        R"nbdoc(
            commit コマンドの設定

            Attributes:
                message: コミットメッセージ
                amend: 直前のコミットを修正するか
                all: 変更済みファイルをすべてステージするか
        )nbdoc"
    )
        .def(nb::init<>())
        .def_rw("message", &commands::CommitConfig::message)
        .def_rw("amend", &commands::CommitConfig::amend)
        .def_rw("all", &commands::CommitConfig::all);

    m.def(
        "run_commit", &commands::runCommit, "config"_a,
        R"nbdoc(
            commit コマンドを実行する

            Args:
                config: CommitConfig

            Raises:
                ValueError: message が空の場合
        )nbdoc"
    );

    nb::class_<commands::FetchConfig>(
        m, "FetchConfig",
        R"nbdoc(
            fetch コマンドの設定

            Attributes:
                remote: 取得元リモート名
                prune: 追跡されなくなったリモートブランチを削除するか
        )nbdoc"
    )
        .def(nb::init<>())
        .def_rw("remote", &commands::FetchConfig::remote)
        .def_rw("prune", &commands::FetchConfig::prune);

    m.def(
        "run_fetch", &commands::runFetch, "config"_a,
        R"nbdoc(
            fetch コマンドを実行する

            Args:
                config: FetchConfig

            Raises:
                ValueError: remote が空の場合
        )nbdoc"
    );

    nb::class_<commands::LogConfig>(
        m, "LogConfig",
        R"nbdoc(
            log コマンドの設定

            Attributes:
                max_count: 表示するコミット数の上限
                paths: 対象を絞り込むパスのリスト
        )nbdoc"
    )
        .def(nb::init<>())
        .def_rw("max_count", &commands::LogConfig::max_count)
        .def_rw("paths", &commands::LogConfig::paths);

    m.def(
        "run_log", &commands::runLog, "config"_a,
        R"nbdoc(
            log コマンドを実行する

            Args:
                config: LogConfig

            Raises:
                ValueError: max_count が正でない場合
        )nbdoc"
    );
}
