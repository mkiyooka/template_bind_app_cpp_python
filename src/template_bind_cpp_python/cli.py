"""Command-line interface.

typerでCLIオプションを解析し、対応するConfig構造体を組み立ててC++実装へ渡す。
"""

from __future__ import annotations

import typer

from template_bind_cpp_python import (
    CommitConfig,  # type: ignore[attr-defined]
    FetchConfig,  # type: ignore[attr-defined]
    LogConfig,  # type: ignore[attr-defined]
    __version__,
    run_commit,  # type: ignore[attr-defined]
    run_fetch,  # type: ignore[attr-defined]
    run_log,  # type: ignore[attr-defined]
)

app = typer.Typer()


@app.command("commit")
def commit(
    message: str = typer.Option(..., "--message", "-m", help="コミットメッセージ"),
    *,
    amend: bool = typer.Option(False, "--amend", help="直前のコミットを修正する"),
    all_: bool = typer.Option(
        False, "--all", "-a", help="変更済みファイルをすべてステージする"
    ),
) -> None:
    """commit コマンドを実行."""
    if run_commit is None or CommitConfig is None:
        print("C++実装が利用できません。'uv pip install -e .' を実行してください")
        raise typer.Exit(1)

    config = CommitConfig()
    config.message = message
    config.amend = amend
    config.all = all_
    run_commit(config)


@app.command("fetch")
def fetch(
    remote: str = typer.Option("origin", "--remote", help="取得元リモート名"),
    *,
    prune: bool = typer.Option(
        False, "--prune", help="追跡されなくなったリモートブランチを削除する"
    ),
) -> None:
    """fetch コマンドを実行."""
    if run_fetch is None or FetchConfig is None:
        print("C++実装が利用できません。'uv pip install -e .' を実行してください")
        raise typer.Exit(1)

    config = FetchConfig()
    config.remote = remote
    config.prune = prune
    run_fetch(config)


@app.command("log")
def log_command(
    max_count: int = typer.Option(
        10, "--max-count", "-n", help="表示するコミット数の上限"
    ),
    paths: list[str] = typer.Option(
        None, "--path", help="対象を絞り込むパス(複数指定可)"
    ),
) -> None:
    """log コマンドを実行."""
    if run_log is None or LogConfig is None:
        print("C++実装が利用できません。'uv pip install -e .' を実行してください")
        raise typer.Exit(1)

    config = LogConfig()
    config.max_count = max_count
    config.paths = paths or []
    run_log(config)


def _version_callback(*, show_version: bool) -> None:
    if show_version:  # pragma: no cover
        print(f"Version: {__version__}")
        raise typer.Exit()


@app.callback()
def main(
    *,
    _version: bool = typer.Option(
        False,
        "--version",
        "-v",
        callback=_version_callback,
        is_eager=True,
        help="バージョンを表示して終了",
    ),
) -> None:
    """コマンド実行テンプレート CLI."""
    return


if __name__ == "__main__":
    app()
