# Yinix

> 一个用 C++17 实现的模拟操作系统，提供交互式命令行 Shell，涵盖进程管理、内存管理、文件系统和设备管理四大核心模块。

---

## 功能概览

| 模块 | 状态 | 说明 |
|---|---|---|
| Shell 主循环 | ✅ 已完成 | REPL 交互式命令行，支持命令路由 |
| 进程管理 | ✅ 已完成 | PCB、状态机、时间片轮转调度 |
| 内存管理 | ✅ 已完成 | First Fit 分配、空闲区合并 |
| 文件系统 | ✅ 已完成 | 内存目录树、路径导航、文件读写 |
| 设备管理 | ✅ 已完成 | 虚拟设备分配与释放 |
| 进程同步/互斥 | ✅ 已完成 | PV 信号量、进程阻塞与唤醒 |

---

## 环境要求

| 工具 | 最低版本 |
|---|---|
| g++ | 11+ |
| CMake | 3.16+ |
| make | 任意 |
| libreadline-dev | 任意 |

Ubuntu / WSL 一键安装：

```bash
sudo apt update && sudo apt install -y g++ cmake make libreadline-dev
```

---

## 构建与运行

```bash
# 克隆仓库
git clone https://github.com/xm2413/yinix.git
cd yinix

# 构建
cmake -B build && cmake --build build

# 运行
./build/yinix
```

---

## 使用示例

启动后进入交互式 Shell：

```
欢迎使用 Yinix OS 模拟器  (输入 help 查看命令)

Yinix:/> help
```

提示符会随当前目录动态变化，例如进入 `/home` 后显示 `Yinix:/home>`。支持上/下方向键翻阅历史命令（readline）。

### 进程管理

```
Yinix> create init 10     # 创建名为 init、优先级10 的进程
Yinix> create bash         # 创建进程 bash
Yinix> ps                  # 列出所有进程
Yinix> run                 # 调度一次（时间片轮转）
Yinix> block 1             # 阻塞 PID=1 的进程
Yinix> wake 1              # 唤醒 PID=1 的进程
Yinix> kill 2              # 终止 PID=2 的进程
```

### 内存管理

```
Yinix> memmap              # 查看内存布局
Yinix> malloc 256 1        # 为进程1分配 256 单位内存
Yinix> malloc 128          # 分配 128 单位内存（无绑定进程）
Yinix> mfree 0             # 释放起始地址为 0 的内存块
Yinix> memmap              # 再次查看，观察合并效果
```

### 文件系统

```
Yinix:/> pwd               # 显示当前路径: /
Yinix:/> mkdir home        # 创建目录 home
Yinix:/> cd home           # 进入 home
Yinix:/home> touch readme.txt    # 创建文件
Yinix:/home> write readme.txt Hello Yinix!  # 写入内容
Yinix:/home> cat readme.txt      # 读取内容
Yinix:/home> ls            # 列出当前目录
Yinix:/home> cd ..         # 返回上级
Yinix:/> rm home           # 删除目录
```

### 设备管理

```
Yinix> devices             # 列出所有设备及状态
Yinix> alloc printer0 1    # 将 printer0 分配给进程1
Yinix> alloc disk0 2       # 将 disk0 分配给进程2
Yinix> release printer0    # 释放 printer0
```

### 进程同步与互斥（PV 信号量）

```
Yinix> sem_create mutex 1  # 创建二元信号量（互斥锁），初始值=1
Yinix> sem_create empty 3  # 创建计数信号量，初始值=3
Yinix> sem_list            # 列出所有信号量及当前值
Yinix> sem_p mutex 1       # 进程1 执行 P(mutex)：value--，不足则阻塞
Yinix> sem_v mutex         # 执行 V(mutex)：value++，有等待进程则唤醒
Yinix> sem_del mutex       # 删除信号量
```

> **说明**：信号量是纯计数器，不与设备或内存块绑定。`sem_p <name> <pid>` 表示让进程 pid 对该信号量执行 P 操作；资源不足时进程自动进入 BLOCKED 状态，V 操作后由系统唤醒。

---

## 项目结构

```
yinix/
├── CMakeLists.txt
└── src/
    ├── main.cpp
    ├── shell/
    │   ├── Shell.h
    │   └── Shell.cpp
    ├── process/
    │   ├── PCB.h
    │   ├── ProcessManager.h
    │   ├── ProcessManager.cpp
    │   ├── Semaphore.h
    │   ├── SyncManager.h
    │   └── SyncManager.cpp
    ├── memory/
    │   ├── MemoryManager.h
    │   └── MemoryManager.cpp
    ├── filesystem/
    │   ├── FSNode.h
    │   ├── FileSystem.h
    │   └── FileSystem.cpp
    └── device/
        ├── Device.h
        ├── DeviceManager.h
        └── DeviceManager.cpp
```

---

## 设计说明

- **进程调度**：时间片轮转，通过 `run` 命令手动触发一次 tick，避免多线程复杂性
- **内存分配**：First Fit 算法，回收时自动合并相邻空闲块，模拟内存大小 1024 单位
- **文件系统**：内存树形结构，文件内容以 `std::string` 存储；支持持久化，退出时自动保存至 `~/osProj/data/yinix_fs.dat`，下次启动自动恢复
- **设备管理**：预置 5 个虚拟设备（printer0、disk0、disk1、keyboard0、screen0）
- **进程同步/互斥**：经典 PV 信号量，`SyncManager` 复用调度层的阻塞/唤醒接口；支持互斥锁（初始值=1）与计数信号量（初始值>1）两种语义
- **集成联动**：`kill` 进程时自动释放其占用的内存和设备
- **Shell 特性**：动态提示符随当前目录变化（`Yinix:/path>`）；基于 readline 支持上/下键历史命令
