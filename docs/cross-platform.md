# クロスプラットフォーム開発ガイド

macOS、Ubuntu、RHEL系（RHEL/CentOS/Alma/Rocky/AmazonLinux2）での開発環境切り替え手順を説明します。

## 🎯 クイックスタート

### 自動セットアップ（推奨）

```bash
# 環境自動検出・セットアップ
./scripts/setup_environment.sh

# 品質管理ツール自動インストール（root権限不要）
./scripts/setup_environment.sh --install-tools

# 手動でプリセット指定
./scripts/setup_environment.sh ubuntu --install-tools
./scripts/setup_environment.sh rhel --install-tools
./scripts/setup_environment.sh macos --install-tools
```

### 手動セットアップ

```bash
# プリセット一覧確認
cmake --list-presets

# 環境別設定・ビルド
cmake --preset=ubuntu             # Ubuntu用
cmake --build --preset=ubuntu-debug

cmake --preset=rhel               # RHEL系用
cmake --build --preset=rhel-debug

cmake --preset=macos              # macOS用
cmake --build --preset=macos-debug
```

## 🔧 環境別設定詳細

### macOS (Homebrew)

**ツール検索パス:**

- `$HOME/.local/bin` (ユーザーローカル)
- `/opt/homebrew/bin` (clang-format)
- `/opt/homebrew/opt/llvm/bin` (LLVM各バージョン)
- `/opt/homebrew/opt/llvm@{14-20}/bin`

**依存関係インストール:**

```bash
# Homebrewを使用（推奨）
brew install cmake cppcheck llvm

# または自動インストール
./scripts/setup_environment.sh --install-tools
```

**特徴:**

- LLVM14環境自動検出・対応
- Homebrew管理ツール優先使用

### Ubuntu/Debian

**ツール検索パス:**

- `$HOME/.local/bin` (ユーザーローカル)
- `/usr/bin` (システム標準)
- `/usr/local/bin` (手動インストール)

**依存関係インストール:**

```bash
# 管理者権限でシステムインストール
sudo apt update
sudo apt install build-essential cmake cppcheck
sudo apt install clang-tidy clang-format  # オプション

# またはroot権限不要のユーザーローカルインストール（推奨）
./scripts/setup_environment.sh --install-tools
```

**追加設定:**

- `--platform=unix64` (cppcheck)

### RHEL系 (RHEL/CentOS/Alma/Rocky/AmazonLinux2)

**ツール検索パス:**

- `$HOME/.local/bin` (ユーザーローカル)
- `/usr/bin` (システム標準)
- `/usr/local/bin` (手動インストール)
- `/opt/rh/llvm-toolset-*/root/usr/bin` (SCL LLVM)

**依存関係インストール:**

```bash
# 管理者権限でシステムインストール
sudo yum update
sudo yum groupinstall "Development Tools"
sudo yum install cmake3 cppcheck

# LLVM toolset (オプション)
sudo yum install centos-release-scl
sudo yum install llvm-toolset-13
scl enable llvm-toolset-13 bash

# またはroot権限不要のユーザーローカルインストール（推奨）
./scripts/setup_environment.sh --install-tools
```

**特徴:**

- RHEL系統合対応 (RHEL/CentOS/Alma/Rocky/AmazonLinux2)
- SCL (Software Collections) LLVM対応
- dnf/yum パッケージ管理

## 🚀 開発ワークフロー

### 1. 環境初期設定

```bash
git clone <repo>
cd template_bind_app_cpp_python
git submodule update --init --recursive
uv sync
```

### 2. 環境固有セットアップ

```bash
# 自動検出
./scripts/setup_environment.sh

# または手動選択
cmake --preset=ubuntu        # 使用環境に応じて
cmake --preset=rhel          # RHEL系環境
```

### 3. 開発作業

```bash
# C++開発・デバッグ
cmake --build build --target debug_main
./build/debug/debug_main

# 品質チェック
cmake --build build --target check

# Python統合テスト
uv pip install -e .
uv run bind-demo commit --message "test"
```

## ⚡ VSCode統合

### タスク実行

1. **Ctrl+Shift+P** → "Tasks: Run Task"
2. **"cmake_configure"** 選択
3. プリセットを選択（例：`ubuntu-debug`）
4. **F5** でデバッグ開始

### launch.json対応

デバッグ設定は全環境共通で使用可能：

- `Debug Main (All Modules)`
- `Debug Main (Commit Command Only)`

## 🛠️ トラブルシューティング

### プリセットが見つからない

```bash
# 現在の環境で利用可能なプリセット確認
cmake --list-presets

# 条件チェック（Linuxの場合のみ表示される）
echo $OSTYPE
```

### ツールが見つからない

```bash
# 個別確認
which cmake
which clang-format
which cppcheck

# パス確認
echo $PATH
```

### ビルドエラー

```bash
# ビルドディレクトリクリーン
rm -rf build/

# 再設定・再ビルド
cmake --preset=<your-preset>
cmake --build --preset=<your-preset>
```

## 🔗 CI/CD統合

```bash
# CI環境用プリセット
cmake --preset=ci
cmake --build --preset=ci
cmake --build build --target test
```

---

**関連ドキュメント:**

- [developer-guide.md](developer-guide.md): 詳細な開発手順
- [user-guide.md](user-guide.md): エンドユーザー向けガイド
