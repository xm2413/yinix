#!/bin/bash
# 同步/互斥（PV 信号量）测试
cd "$(dirname "$0")"
source helper.sh

echo "=== 同步/互斥测试 ==="

# ---- 基础操作 ----
assert_contains "创建信号量成功" "已创建" \
"sem_create mutex 1
exit"

assert_contains "sem_list 列出信号量" "mutex" \
"sem_create mutex 1
sem_list
exit"

assert_contains "sem_list 显示初始值" "1" \
"sem_create mutex 1
sem_list
exit"

assert_contains "删除信号量成功" "已删除" \
"sem_create mutex 1
sem_del mutex
exit"

assert_contains "重复创建同名信号量报错" "已存在" \
"sem_create mutex 1
sem_create mutex 1
exit"

assert_contains "删除不存在的信号量报错" "不存在" \
"sem_del ghost
exit"

# ---- P 操作（互斥锁语义：初始值 1）----
assert_contains "P 操作成功获得资源（value=0）" "成功获得资源" \
"sem_create mutex 1
create p1
sem_p mutex 1
exit"

assert_contains "P 操作后 value 变为 0" "value=0" \
"sem_create mutex 1
create p1
sem_p mutex 1
exit"

assert_contains "P 操作资源不足时阻塞进程" "被阻塞" \
"sem_create mutex 1
create p1
create p2
sem_p mutex 1
sem_p mutex 2
exit"

assert_contains "P 操作后进程状态变为 BLOCKED" "BLOCKED" \
"sem_create mutex 1
create p1
create p2
sem_p mutex 1
sem_p mutex 2
ps
exit"

# ---- V 操作 ----
assert_contains "V 操作唤醒等待进程" "唤醒" \
"sem_create mutex 1
create p1
create p2
sem_p mutex 1
sem_p mutex 2
sem_v mutex
exit"

assert_contains "V 操作后被阻塞进程变为 READY" "READY" \
"sem_create mutex 1
create p1
create p2
sem_p mutex 1
sem_p mutex 2
sem_v mutex
ps
exit"

assert_contains "无等待进程时 V 操作正常归还" "无等待进程" \
"sem_create mutex 1
create p1
sem_p mutex 1
sem_v mutex
exit"

# ---- 计数信号量语义（初始值 > 1）----
assert_contains "计数信号量允许多个进程同时获得资源" "成功获得资源" \
"sem_create pool 3
create p1
create p2
create p3
sem_p pool 1
sem_p pool 2
sem_p pool 3
exit"

assert_contains "计数信号量耗尽后阻塞" "被阻塞" \
"sem_create pool 2
create p1
create p2
create p3
sem_p pool 1
sem_p pool 2
sem_p pool 3
exit"

print_summary
