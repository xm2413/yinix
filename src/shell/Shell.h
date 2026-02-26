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
    void run();  // 启动主循环

private:
    ProcessManager procMgr;
    MemoryManager  memMgr;
    FileSystem     fs;
    DeviceManager  devMgr;

    void dispatch(const std::vector<std::string>& args);
    void printHelp();
    std::vector<std::string> tokenize(const std::string& line);

    // 各模块命令处理
    void handleProc(const std::vector<std::string>& args);
    void handleMem (const std::vector<std::string>& args);
    void handleFS  (const std::vector<std::string>& args);
    void handleDev (const std::vector<std::string>& args);
};
