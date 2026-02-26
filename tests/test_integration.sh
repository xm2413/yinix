#!/bin/bash
# 集成联动测试：kill 进程自动释放内存和设备
cd "$(dirname "$0")"
source helper.sh

echo "=== 集成联动测试 ==="

assert_contains "kill 进程时自动释放内存" "自动释放" \
"create worker
malloc 256 1
kill 1
exit"

assert_contains "kill 进程时自动释放设备" "自动释放" \
"create worker
alloc printer0 1
kill 1
exit"

assert_contains "kill 后内存变回空闲" "空闲" \
"create worker
malloc 256 1
kill 1
memmap
exit"

assert_contains "kill 后设备变回空闲" "空闲" \
"create worker
alloc disk0 1
kill 1
devices
exit"

assert_contains "kill 后内存可再次分配" "分配成功" \
"create worker
malloc 512 1
kill 1
malloc 512
exit"

assert_contains "多进程各占资源互不影响" "分配成功" \
"create p1
create p2
malloc 256 1
malloc 256 2
alloc printer0 1
kill 1
malloc 256
exit"

print_summary
