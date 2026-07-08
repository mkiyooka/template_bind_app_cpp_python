import pytest

import template_bind_cpp_python as mod  # ignore[import]


def test_run_commit_succeeds() -> None:
    """Test run_commit binding with a valid message"""
    config = mod.CommitConfig()
    config.message = "test commit"
    config.amend = False
    config.all = True

    mod.run_commit(config)  # should not raise


def test_run_commit_rejects_empty_message() -> None:
    """Test run_commit binding raises ValueError for empty message"""
    config = mod.CommitConfig()
    config.message = ""

    with pytest.raises(ValueError, match="commit message must not be empty"):
        mod.run_commit(config)


def test_run_fetch_succeeds() -> None:
    """Test run_fetch binding with default remote"""
    config = mod.FetchConfig()

    mod.run_fetch(config)  # should not raise


def test_run_fetch_rejects_empty_remote() -> None:
    """Test run_fetch binding raises ValueError for empty remote"""
    config = mod.FetchConfig()
    config.remote = ""

    with pytest.raises(ValueError, match="fetch remote must not be empty"):
        mod.run_fetch(config)


def test_run_log_succeeds_with_paths() -> None:
    """Test run_log binding with max_count and paths"""
    config = mod.LogConfig()
    config.max_count = 5
    config.paths = ["src", "tests"]

    mod.run_log(config)  # should not raise


def test_run_log_rejects_non_positive_max_count() -> None:
    """Test run_log binding raises ValueError for non-positive max_count"""
    config = mod.LogConfig()
    config.max_count = 0

    with pytest.raises(ValueError, match="log max_count must be positive"):
        mod.run_log(config)
