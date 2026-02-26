# Yinix —— C++ 操作系统模拟课程设计计划

**总览**：构建一个名为 **Yinix** 的模拟操作系统，提供交互式命令行 Shell，内嵌四个相互独立的子系统模块（进程管理、内存管理、文件系统、设备管理），每个模块在内存中以数据结构模拟 OS 行为，无需真实内核调用。用户通过 Shell 输入命令驱动各模块演示。

---

## 项目结构

```
osProj/
├── CMakeLists.txt
├── src/
│   ├── main.cpp                 # 入口：启动 Shell 主循环
│   ├── shell/
│   │   ├── Shell.h/.cpp         # 命令解析与分发
│   ├── process/
│   │   ├── PCB.h                # 进程控制块定义
│   │   └── ProcessManager.h/.cpp
│   ├── memory/
│   │   ├── MemoryManager.h/.cpp # 内存分配/回收
│   ├── filesystem/
│   │   ├── FSNode.h             # 文件/目录节点定义
│   │   └── FileSystem.h/.cpp
│   └── device/
│       ├── Device.h             # 设备定义
│       └── DeviceManager.h/.cpp
└── README.md
```

---

## 环境要求与配置

### 所需工具

| 工具 | 最低版本 | 用途 |
|---|---|---|
| g++ | 11+ | C++17 编译器 |
| CMake | 3.16+ | 构建系统 |
| make | 任意 | 执行 CMake 生成的构建文件 |

### 当前环境检查（Ubuntu 24.04.1 LTS / WSL2）

| 工具 | 状态 |
|---|---|
| g++ | ✅ 13.3.0 |
| CMake | ✅ 3.28.3 |
| make | ✅ 4.3 |

### 一键安装命令

```bash
sudo apt update && sudo apt install -y g++ cmake make
```

安装完成后可验证：

```bash
g++ --version
cmake --version
make --version
```

---

## 实现步骤

### 1. 初始化项目
创建上述目录结构，编写 `CMakeLists.txt`，配置 C++17，所有 `src/` 下源文件参与编译。

**执行记录：**
- [x] 确认环境：g++ 13.3.0 / CMake 3.28.3 / make 4.3
- [x] 创建 `src/shell/`、`src/process/`、`src/memory/`、`src/filesystem/`、`src/device/` 目录
- [x] 编写 `CMakeLists.txt`，启用 C++17，递归收集所有 `.cpp` 源文件，输出可执行文件 `yinix`
- [x] 为各模块创建带最小骨架的占位头文件与源文件（可通过编译）
- [x] 执行 `cmake -B build && cmake --build build` 验证初始编译通过

> ✅ **步骤 1 已完成**，`build/yinix` 编译成功，所有模块骨架已就位。

### 2. Shell 主循环
`src/main.cpp` 启动后显示 `Yinix>` 提示符并进入 REPL，读取用户输入，由 `Shell` 类解析命令名+参数后分发给对应子模块；支持 `help`、`exit` 全局命令，其余命令按前缀路由（如 `ps`/`proc`/`mem`/`fs`/`dev`）。

### 3. 进程管理模块
- `PCB.h`：定义 `PCB` 结构体，字段包括 `pid`、`name`、`state`（枚举 NEW/READY/RUNNING/BLOCKED/TERMINATED）、`priority`、`cpu_time`
- `ProcessManager`：维护进程表（`map<int,PCB>`）和就绪队列；实现创建、撤销、状态转换（就绪→运行→阻塞→就绪）、简单时间片轮转调度（模拟 tick）
- Shell 命令：
  - `ps` — 列出所有进程
  - `create <name>` — 创建进程
  - `kill <pid>` — 撤销进程
  - `run` — 调度一次 tick
  - `block <pid>` — 阻塞进程
  - `wake <pid>` — 唤醒进程

### 4. 内存管理模块
- 模拟固定大小内存（如 1024 单位），用**空闲区链表**表示已分配/空闲块
- 实现 **First Fit** 分配策略 + 回收时相邻合并
- Shell 命令：
  - `memmap` — 可视化内存布局
  - `malloc <size>` — 返回分配起始地址
  - `free <addr>` — 释放内存块

### 5. 文件系统模块
- 用树形结构（`FSNode`）在内存中模拟目录树，节点含：名称、类型（文件/目录）、子节点、文件内容字符串、创建时间
- 维护当前工作目录指针，支持相对路径解析
- Shell 命令：
  - `ls` — 列出当前目录
  - `mkdir <name>` — 创建目录
  - `cd <path>` — 切换目录
  - `touch <name>` — 创建空文件
  - `write <name> <content>` — 写文件内容
  - `read <name>` — 读文件内容
  - `rm <name>` — 删除文件或目录
  - `pwd` — 显示当前路径

### 6. 设备管理模块
- 预置若干虚拟设备（打印机、磁盘、键盘等），用 `Device` 结构体存储：名称、状态（IDLE/BUSY）、占用的 PID
- 实现设备申请（绑定进程）与释放，申请时若设备忙则拒绝并提示
- Shell 命令：
  - `devices` — 列出所有设备状态
  - `alloc <device> <pid>` — 申请设备
  - `release <device>` — 释放设备

### 7. 集成联动（可选演示增强）
- 进程被 `kill` 时自动释放其占用的设备和内存
- `malloc` 返回的地址可绑定到某个 PID，便于整体演示

---

## 编译与运行

```bash
cmake -B build && cmake --build build
./build/yinix
```

---

## 设计决策说明

| 方面 | 决策 | 原因 |
|---|---|---|
| 内存分配算法 | First Fit | 实现最简单，演示效果直观 |
| 调度算法 | 手动触发时间片轮转（`run` 命令） | 避免多线程复杂性 |
| 文件内容存储 | `std::string` 字段 | 无需模拟 inode/block，够演示即可 |
| 数据持久化 | 不持久化，内存结构 | 课程设计无需持久化 |
| 编程语言 | C++17 | 课程要求 |

---

## 错误处理要求

- kill 不存在的进程 → 友好错误信息
- free 未分配地址 → 友好错误信息
- cd 到不存在目录 → 友好错误信息
- alloc 已 BUSY 的设备 → 提示当前占用 PID
