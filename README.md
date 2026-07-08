# C++ テンプレートバインディングプロジェクト (nanobind)

C++とPythonをnanobindでバインディングするテンプレートプロジェクトです。「main関数のようなコマンド単位をPythonから呼び出す」形式を採用しており、Python側はtyperでCLIオプションを解析して設定をConfig構造体に格納し、C++側の対応する実行関数へ渡すだけの役割を担います。

## クイックスタート

```bash
git clone <repo>
cd <dir>
git submodule update --init --recursive
uv sync
unset CC CXX  # LLVM環境がある場合はリセット
uv pip install -e .
bind-demo commit --message "hello" --all
```

### 💡 トラブルシューティング

**モジュールインポートエラーが発生する場合:**

```bash
# ビルド環境をリセットして再ビルド
unset CC CXX
rm -rf build/ .venv/lib/python*/site-packages/template_bind_cpp_python*
uv pip install -e .
```

## 機能

### コマンドサンプル (commit / fetch / log)

git の commit / fetch / log 相当のダミー実装です。実際のgit操作は行わず、標準出力にメッセージを出すだけのサンプルとして、コマンド単位でPythonからC++を呼び出す設計を示します。

- `bind-demo commit --message "hello" --all` : コミットのダミー実行（`--amend`、`--all` オプション対応）
- `bind-demo fetch --remote upstream --prune` : フェッチのダミー実行（`--remote`、`--prune` オプション対応）
- `bind-demo log --max-count 3 --path src --path tests` : ログ表示のダミー実行（`--max-count`、複数指定可能な `--path` オプション対応）

異常系はC++側で `std::invalid_argument` 等の例外を投げ、nanobindが自動的にPython例外（`ValueError` 等）に変換します。正常系の戻り値はありません（`void`）。

### 新しいコマンドの追加手順

このプロジェクトはテンプレートのため、新しいコマンドを追加する際は以下の流れに従います。

1. `include/commands/` に `XxxConfig` 構造体と `runXxx` 関数の宣言を追加する
2. `src/core/commands/` に `runXxx` の実装を追加する
3. `src/bindings/nb_bindings.cpp` に `XxxConfig` と `runXxx` のバインディングを追加する
4. `src/template_bind_cpp_python/cli.py` にtyperのサブコマンドを追加し、オプションから `XxxConfig` を組み立てて `runXxx` を呼び出す

### クロスプラットフォーム開発環境

設計思想: ビルドと品質ツールの分離

- **ビルド**: macOS (Apple Clang)、Linux (GCC) - 安定性重視
- **品質ツール**: 全環境でLLVM (clang-format, clang-tidy, scan-build) - 最新機能活用
- **プリセット**: 環境別に最適化された設定を自動適用
- **自動構築**: `scripts/setup_environment.sh` で環境検出とツール自動インストール（root権限不要）

## 開発用ビルド

### pixi を使った開発（推奨）

```bash
pixi install

pixi run config        # cmake --preset=release (Ninja)
pixi run build         # cmake --build build
pixi run test          # ctest
pixi run clean         # ビルドディレクトリをクリーン
```

ビルド成果物は `build/` に出力されます。

- スタティックライブラリ: `build/src/core/libcxx_core.a`
- Python バインディング (.so): `build/src/bindings/_nanobind_module*.so`

### サニタイザ / カバレッジ / Valgrind（Linux）

```bash
# ASan + UBSan（GCC）
pixi run asan

# カバレッジレポート（clang、build-coverage/coverage-html/index.html に生成）
pixi run coverage

# Valgrind（要: sudo apt-get install valgrind）
pixi run valgrind
```

### 品質チェック

```bash
pixi run fullcheck     # typos + clang-tidy + cppcheck
pixi run lint          # clang-tidy
pixi run run-cppcheck  # cppcheck
pixi run format        # clang-format
```

### 環境別プリセット使用（pixi 未使用時）

```bash
# Ubuntu環境 (GCC + LLVM品質ツール)
cmake --preset=ubuntu
cmake --build --preset=ubuntu-debug

# macOS環境 (Apple Clang + Homebrew LLVM)
cmake --preset=macos
cmake --build --preset=macos-debug
```

## 技術仕様

- **バインディング**: nanobind (submodule)
- **ビルドシステム**: scikit-build-core + CMake
- **パッケージ管理**: uv
- **C++標準**: C++17

## 開発者向け

- [ユーザ向けガイド](docs/user-guide.md)
- [開発者向けガイド](docs/developer-guide.md)
- [クロスプラットフォーム開発](docs/cross-platform.md)
- [開発ルール](docs/development-rules.md)
- [このプロジェクトの実装状況](docs/implementation-status.md)

## License

This project is released under CC0 1.0 Universal.
You can copy, modify, and use it freely without attribution.
