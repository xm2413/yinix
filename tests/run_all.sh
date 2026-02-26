#!/bin/bash
# 运行所有测试
cd "$(dirname "$0")"

# 先确保可执行文件存在
if [ ! -f ../build/yinix ]; then
    echo "未找到 ../build/yinix，正在构建..."
    cd .. && cmake --build build && cd tests
fi

TOTAL_PASS=0
TOTAL_FAIL=0

run_suite() {
    local out
    out=$(bash "$1" 2>&1)
    echo "$out"
    local p f
    p=$(echo "$out" | grep -oP '通过: \K[0-9]+' | tail -1)
    f=$(echo "$out" | grep -oP '失败: \K[0-9]+' | tail -1)
    TOTAL_PASS=$((TOTAL_PASS + ${p:-0}))
    TOTAL_FAIL=$((TOTAL_FAIL + ${f:-0}))
}

echo "╔══════════════════════════════════════╗"
echo "║        Yinix 功能测试套件             ║"
echo "╚══════════════════════════════════════╝"
echo ""

run_suite test_process.sh
echo ""
run_suite test_memory.sh
echo ""
run_suite test_filesystem.sh
echo ""
run_suite test_device.sh
echo ""
run_suite test_integration.sh

echo ""
echo "╔══════════════════════════════════════╗"
printf "║  总计  通过: %-5s  失败: %-5s        ║\n" "$TOTAL_PASS" "$TOTAL_FAIL"
echo "╚══════════════════════════════════════╝"

[ $TOTAL_FAIL -eq 0 ] && exit 0 || exit 1
