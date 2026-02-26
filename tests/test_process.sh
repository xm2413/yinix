#!/bin/bash
# 进程管理测试
cd "$(dirname "$0")"
source helper.sh

echo "=== 进程管理测试 ==="

assert_contains "创建进程返回 PID" "进程已创建" \
"create init
exit"

assert_contains "ps 列出进程" "init" \
"create init
ps
exit"

assert_contains "进程初始状态为 READY" "READY" \
"create init
ps
exit"

assert_contains "run 后进程变为 RUNNING" "RUNNING" \
"create init
run
ps
exit"

assert_contains "block 阻塞进程" "已阻塞" \
"create init
run
block 1
exit"

assert_contains "wake 唤醒进程" "已唤醒" \
"create init
run
block 1
wake 1
exit"

assert_contains "kill 终止进程" "已终止" \
"create init
kill 1
exit"

assert_contains "kill 不存在进程报错" "不存在" \
"kill 999
exit"

assert_contains "多进程轮转调度" "获得 CPU" \
"create p1
create p2
create p3
run
run
run
exit"

print_summary
