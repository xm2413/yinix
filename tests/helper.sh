#!/bin/bash
# 测试辅助函数

YINIX=../build/yinix
PASS=0
FAIL=0

# 运行一组命令，检查输出中是否包含期望字符串
# 用法: assert_contains "期望字符串" <<EOF ... EOF
assert_contains() {
    local desc="$1"
    local expected="$2"
    local input="$3"
    local output
    output=$(echo "$input" | $YINIX 2>&1)
    if echo "$output" | grep -q "$expected"; then
        echo "  ✅ PASS: $desc"
        ((PASS++))
    else
        echo "  ❌ FAIL: $desc"
        echo "     期望包含: $expected"
        echo "     实际输出: $(echo "$output" | tail -5)"
        ((FAIL++))
    fi
}

# 打印最终统计
print_summary() {
    echo ""
    echo "═══════════════════════════"
    echo "  通过: $PASS  失败: $FAIL"
    echo "═══════════════════════════"
    [ $FAIL -eq 0 ] && exit 0 || exit 1
}
