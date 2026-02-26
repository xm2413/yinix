#!/bin/bash
# 内存管理测试
cd "$(dirname "$0")"
source helper.sh

echo "=== 内存管理测试 ==="

assert_contains "初始内存全部空闲" "空闲" \
"memmap
exit"

assert_contains "malloc 分配成功" "分配成功" \
"malloc 256
exit"

assert_contains "malloc 后出现占用块" "占用" \
"malloc 256
memmap
exit"

assert_contains "mfree 释放成功" "释放成功" \
"malloc 256
mfree 0
exit"

assert_contains "释放后空闲块合并" "1024" \
"malloc 256
malloc 256
mfree 0
mfree 256
memmap
exit"

assert_contains "内存不足时报错" "内存不足" \
"malloc 2000
exit"

assert_contains "释放不存在地址报错" "未被分配" \
"mfree 999
exit"

assert_contains "malloc 绑定 PID" "1" \
"malloc 128 1
memmap
exit"

print_summary
