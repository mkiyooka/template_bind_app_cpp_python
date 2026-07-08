"""Command Binding Template

CLI(typer)で解析した設定をC++実装のコマンドへ渡すテンプレートプロジェクトです。

Example:
    >>> import template_bind_cpp_python as lib
    >>> config = lib.CommitConfig()
    >>> config.message = "initial commit"
    >>> lib.run_commit(config)
"""

from __future__ import annotations

from importlib.metadata import version as get_version

try:
    from ._nanobind_module import (  # type: ignore[import-untyped]
        CommitConfig,
        FetchConfig,
        LogConfig,
        run_commit,
        run_fetch,
        run_log,
    )
except ImportError:
    # モジュールがビルドされていない場合のフォールバック
    CommitConfig = None  # type: ignore[assignment,misc]
    FetchConfig = None  # type: ignore[assignment,misc]
    LogConfig = None  # type: ignore[assignment,misc]
    run_commit = None  # type: ignore[assignment]
    run_fetch = None  # type: ignore[assignment]
    run_log = None  # type: ignore[assignment]

__version__ = get_version("template_bind_cpp_python")

__all__ = [
    "CommitConfig",
    "FetchConfig",
    "LogConfig",
    "run_commit",
    "run_fetch",
    "run_log",
]
