#!/usr/bin/env python3
"""docstringとホバーヒント確認用スクリプト"""

try:
    import template_bind_app_cpp_python as lib

    # ヘルプ表示確認
    print("=== HammingDistanceCalculator ヘルプ ===")
    print(lib.HammingDistanceCalculator.__doc__)

    print("\n=== __init__ ヘルプ ===")
    print(lib.HammingDistanceCalculator.__init__.__doc__)

    print("\n=== calculate ヘルプ ===")
    print(lib.HammingDistanceCalculator([1, 2], [3, 4]).calculate.__doc__)

    print("\n=== 実際の使用例 ===")
    # 実際の使用例 (VSCodeでホバー確認用)
    calc = lib.HammingDistanceCalculator([1, 2, 3], [4, 5, 6])  # ← ここでホバー
    result = calc.calculate()  # ← ここでホバー
    size = calc.size()  # ← ここでホバー

    print(f"Result: {result}, Size: {size}")

except ImportError as e:
    print(f"モジュールが利用できません: {e}")
    print("'uv pip install -e .' を実行してからお試しください")
