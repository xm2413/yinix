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

---

## 环境要求

| 工具 | 最低版本 |
|---|---|
| g++ | 11+ |
| CMake | 3.16+ |
| make | 任意 |

Ubuntu / WSL 一键安装：

```bash
sudo apt update && sudo apt install -y g++ cmake make
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

Yinix> help
```

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
Yinix> pwd                 # 显示当前路径: /
Yinix> mkdir home          # 创建目录 home
Yinix> cd home             # 进入 home
Yinix> touch readme.txt    # 创建文件
Yinix> write readme.txt Hello Yinix!  # 写入内容
Yinix> cat readme.txt      # 读取内容
Yinix> ls                  # 列出当前目录
Yinix> cd ..               # 返回上级
Yinix> rm home             # 删除目录
```

### 设备管理

```
Yinix> devices             # 列出所有设备及状态
Yinix> alloc printer0 1    # 将 printer0 分配给进程1
Yinix> alloc disk0 2       # 将 disk0 分配给进程2
Yinix> release printer0    # 释放 printer0
```

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
    │   └── ProcessManager.cpp
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
- **文件系统**：内存树形结构，文件内容以 `std::string` 存储，无磁盘持久化
- **设备管理**：预置 5 个虚拟设备（printer0、disk0、disk1、keyboard0、screen0）
- **数据持久化**：无，程序退出后所有数据清空（课程演示用途）
