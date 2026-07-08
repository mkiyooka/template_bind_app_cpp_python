# ユーザーガイド

C++コマンド実行バインディングプロジェクトのエンドユーザー向け利用ガイドです。

## 📦 インストール

### ローカルインストール

```bash
git clone <repo>
cd template_bind_app_cpp_python
git submodule update --init --recursive
uv pip install -e .
```

### リモートインストール

```bash
uv pip install git+https://github.com/user/template_bind_app_cpp_python.git
```

## 🚀 基本的な使い方

### CLI経由での実行

インストールすると `bind-demo` コマンドが使えるようになります。

```bash
# commit コマンド
bind-demo commit --message "hello" --all

# fetch コマンド
bind-demo fetch --remote upstream --prune

# log コマンド（複数パス指定可）
bind-demo log --max-count 3 --path src --path tests
```

### Pythonから直接呼び出す

CLIを経由せず、Configオブジェクトを組み立ててC++実装を直接呼び出すこともできます。

```python
import template_bind_app_cpp_python as lib

# commit
config = lib.CommitConfig()
config.message = "initial commit"
config.amend = False
config.all = True
lib.run_commit(config)  # 失敗時は ValueError が送出される

# fetch
fetch_config = lib.FetchConfig()
fetch_config.remote = "origin"
fetch_config.prune = False
lib.run_fetch(fetch_config)

# log
log_config = lib.LogConfig()
log_config.max_count = 10
log_config.paths = ["src", "tests"]
lib.run_log(log_config)
```

## 🔧 型ヒント対応

インストール後は型ヒントが利用可能になります。

```python
from template_bind_app_cpp_python import CommitConfig, run_commit


def commit_all(message: str) -> None:
    config = CommitConfig()
    config.message = message
    config.all = True
    run_commit(config)
```

## 🛠️ トラブルシューティング

### インストールエラー

- C++コンパイラが必要です（gcc/clang）
- submodule更新を忘れずに実行してください

### コマンド実行時のエラー

- `ValueError` が発生する場合、Config構造体の値（メッセージが空、max_countが0以下など）を確認してください
- C++実装が利用できない場合は `uv pip install -e .` を再実行してください
