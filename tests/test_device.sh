#!/bin/bash
# 设备管理测试
cd "$(dirname "$0")"
source helper.sh

echo "=== 设备管理测试 ==="

assert_contains "devices 列出所有设备" "printer0" \
"devices
exit"

assert_contains "初始设备全部空闲" "空闲" \
"devices
exit"

assert_contains "alloc 分配设备" "已分配" \
"alloc printer0 1
exit"

assert_contains "分配后设备变为占用" "占用" \
"alloc printer0 1
devices
exit"

assert_contains "release 释放设备" "已释放" \
"alloc printer0 1
release printer0
exit"

assert_contains "重复分配忙碌设备报错" "占用" \
"alloc disk0 1
alloc disk0 2
exit"

assert_contains "release 不存在设备报错" "不存在" \
"release fakedev
exit"

print_summary
