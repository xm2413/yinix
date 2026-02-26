#pragma once
#include <string>
#include <vector>
#include "process/ProcessManager.h"
#include "memory/MemoryManager.h"
#include "filesystem/FileSystem.h"
#include "device/DeviceManager.h"

class Shell {
public:
    Shell();
    void run();

private:
    ProcessManager procMgr;
    MemoryManager  memMgr;
    FileSystem     fs;
    DeviceManager  devMgr;
    std::string    fsSavePath;  // 文件系统持久化路径
    bool           interactive; // 是否为交互式终端（非管道时不保存）

    void dispatch(const std::vector<std::string>& args);
    void printHelp();
    std::vector<std::string> tokenize(const std::string& line);

    void handleProc(const std::vector<std::string>& args);
    void handleMem (const std::vector<std::string>& args);
    void handleFS  (const std::vector<std::string>& args);
    void handleDev (const std::vector<std::string>& args);
};
