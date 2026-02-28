#pragma once
#include <string>
#include <vector>
#include "process/ProcessManager.h"
#include "process/SyncManager.h"
#include "memory/MemoryManager.h"
#include "filesystem/FileSystem.h"
#include "device/DeviceManager.h"

// Shell 负责：读取命令、解析参数、路由到四大管理模块。
class Shell {
public:
    // 初始化运行环境：确定交互模式、准备持久化路径、按需加载文件系统。
    Shell();

    // REPL 主循环。
    void run();

private:
    // 核心模块管理器（声明顺序即初始化顺序：syncMgr 需引用 procMgr，须在其后）。
    ProcessManager procMgr;
    SyncManager    syncMgr;  // 同步/互斥管理器
    MemoryManager  memMgr;
    FileSystem     fs;
    DeviceManager  devMgr;

    // 文件系统持久化文件路径（默认：~/osProj/data/yinix_fs.dat）。
    std::string    fsSavePath;  // 文件系统持久化路径

    // 是否为交互式终端：交互模式会自动加载/保存；管道模式不持久化。
    bool           interactive;

    // 命令分发入口。
    void dispatch(const std::vector<std::string>& args);

    // 打印帮助信息。
    void printHelp();

    // 按空白拆分输入行为 token 列表。
    std::vector<std::string> tokenize(const std::string& line);

    // 各模块命令处理器。
    void handleProc(const std::vector<std::string>& args);
    void handleMem (const std::vector<std::string>& args);
    void handleFS  (const std::vector<std::string>& args);
    void handleDev (const std::vector<std::string>& args);
    void handleSync(const std::vector<std::string>& args);  // 同步/互斥命令
};
