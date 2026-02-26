#!/bin/bash
# 文件系统测试
cd "$(dirname "$0")"
source helper.sh

echo "=== 文件系统测试 ==="

assert_contains "pwd 初始为根目录" "/" \
"pwd
exit"

assert_contains "mkdir 创建目录" "已创建" \
"mkdir home
exit"

assert_contains "ls 列出目录" "home" \
"mkdir home
ls
exit"

assert_contains "cd 进入目录" "home" \
"mkdir home
cd home
pwd
exit"

assert_contains "cd .. 返回上级" "/" \
"mkdir home
cd home
cd ..
pwd
exit"

assert_contains "touch 创建文件" "已创建" \
"touch test.txt
exit"

assert_contains "write 写入内容" "写入成功" \
"touch test.txt
write test.txt Hello Yinix
exit"

assert_contains "cat 读取内容" "Hello Yinix" \
"touch test.txt
write test.txt Hello Yinix
cat test.txt
exit"

assert_contains "rm 删除文件" "已删除" \
"touch test.txt
rm test.txt
exit"

assert_contains "cd 不存在目录报错" "不存在" \
"cd nonexistent
exit"

assert_contains "cat 不存在文件报错" "不存在" \
"cat ghost.txt
exit"

assert_contains "重复创建报错" "已存在" \
"mkdir dup
mkdir dup
exit"

print_summary
