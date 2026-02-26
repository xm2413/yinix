#include "Shell.h"
#include <iostream>
#include <sstream>

Shell::Shell() {}

// ─────────────────────────────────────────────
// 工具：将输入行拆分为 token 列表
// ─────────────────────────────────────────────
std::vector<std::string> Shell::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// ─────────────────────────────────────────────
// help
// ─────────────────────────────────────────────
void Shell::printHelp() {
    std::cout <<
        "\n=== Yinix Help ===\n"
        "\n[全局]\n"
        "  help                    显示帮助\n"
        "  exit                    退出\n"
        "\n[进程管理]\n"
        "  ps                      列出所有进程\n"
        "  create <name> [pri]     创建进程（可选优先级，默认0）\n"
        "  kill   <pid>            终止进程\n"
        "  block  <pid>            阻塞进程\n"
        "  wake   <pid>            唤醒进程\n"
        "  run                     调度一次（时间片轮转 tick）\n"
        "\n[内存管理]\n"
        "  memmap                  显示内存布局\n"
        "  malloc <size> [pid]     分配内存（可绑定进程）\n"
        "  mfree  <addr>           释放内存\n"
        "\n[文件系统]\n"
        "  pwd                     显示当前路径\n"
        "  ls                      列出当前目录\n"
        "  cd     <path>           切换目录（支持 .. 和 /）\n"
        "  mkdir  <name>           创建目录\n"
        "  touch  <name>           创建空文件\n"
        "  write  <name> <内容>    写入文件内容\n"
        "  cat    <name>           读取文件内容\n"
        "  rm     <name>           删除文件或目录\n"
        "\n[设备管理]\n"
        "  devices                 列出所有设备\n"
        "  alloc  <设备名> <pid>   分配设备给进程\n"
        "  release <设备名>        释放设备\n"
        "\n";
}

// ─────────────────────────────────────────────
// 进程管理命令
// ─────────────────────────────────────────────
void Shell::handleProc(const std::vector<std::string>& args) {
    const std::string& cmd = args[0];
    if (cmd == "ps") {
        procMgr.listProcesses();
    } else if (cmd == "create") {
        if (args.size() < 2) { std::cout << "用法: create <name> [priority]\n"; return; }
        int pri = (args.size() >= 3) ? std::stoi(args[2]) : 0;
        procMgr.createProcess(args[1], pri);
    } else if (cmd == "kill") {
        if (args.size() < 2) { std::cout << "用法: kill <pid>\n"; return; }
        procMgr.killProcess(std::stoi(args[1]));
    } else if (cmd == "block") {
        if (args.size() < 2) { std::cout << "用法: block <pid>\n"; return; }
        procMgr.blockProcess(std::stoi(args[1]));
    } else if (cmd == "wake") {
        if (args.size() < 2) { std::cout << "用法: wake <pid>\n"; return; }
        procMgr.wakeProcess(std::stoi(args[1]));
    } else if (cmd == "run") {
        procMgr.tick();
    }
}

// ─────────────────────────────────────────────
// 内存管理命令
// ─────────────────────────────────────────────
void Shell::handleMem(const std::vector<std::string>& args) {
    const std::string& cmd = args[0];
    if (cmd == "memmap") {
        memMgr.printMap();
    } else if (cmd == "malloc") {
        if (args.size() < 2) { std::cout << "用法: malloc <size> [pid]\n"; return; }
        int size = std::stoi(args[1]);
        int pid  = (args.size() >= 3) ? std::stoi(args[2]) : -1;
        memMgr.allocate(size, pid);
    } else if (cmd == "mfree") {
        if (args.size() < 2) { std::cout << "用法: mfree <addr>\n"; return; }
        memMgr.release(std::stoi(args[1]));
    }
}

// ─────────────────────────────────────────────
// 文件系统命令
// ─────────────────────────────────────────────
void Shell::handleFS(const std::vector<std::string>& args) {
    const std::string& cmd = args[0];
    if (cmd == "pwd") {
        fs.pwd();
    } else if (cmd == "ls") {
        fs.ls();
    } else if (cmd == "cd") {
        if (args.size() < 2) { std::cout << "用法: cd <path>\n"; return; }
        fs.cd(args[1]);
    } else if (cmd == "mkdir") {
        if (args.size() < 2) { std::cout << "用法: mkdir <name>\n"; return; }
        fs.mkdir(args[1]);
    } else if (cmd == "touch") {
        if (args.size() < 2) { std::cout << "用法: touch <name>\n"; return; }
        fs.touch(args[1]);
    } else if (cmd == "write") {
        if (args.size() < 3) { std::cout << "用法: write <name> <内容>\n"; return; }
        std::string content;
        for (size_t i = 2; i < args.size(); ++i) {
            if (i > 2) content += ' ';
            content += args[i];
        }
        fs.write(args[1], content);
    } else if (cmd == "cat") {
        if (args.size() < 2) { std::cout << "用法: cat <name>\n"; return; }
        fs.read(args[1]);
    } else if (cmd == "rm") {
        if (args.size() < 2) { std::cout << "用法: rm <name>\n"; return; }
        fs.rm(args[1]);
    }
}

// ─────────────────────────────────────────────
// 设备管理命令
// ─────────────────────────────────────────────
void Shell::handleDev(const std::vector<std::string>& args) {
    const std::string& cmd = args[0];
    if (cmd == "devices") {
        devMgr.listDevices();
    } else if (cmd == "alloc") {
        if (args.size() < 3) { std::cout << "用法: alloc <设备名> <pid>\n"; return; }
        devMgr.allocate(args[1], std::stoi(args[2]));
    } else if (cmd == "release") {
        if (args.size() < 2) { std::cout << "用法: release <设备名>\n"; return; }
        devMgr.release(args[1]);
    }
}

// ─────────────────────────────────────────────
// 主分发器
// ─────────────────────────────────────────────
void Shell::dispatch(const std::vector<std::string>& args) {
    if (args.empty()) return;
    const std::string& cmd = args[0];

    // 全局命令
    if (cmd == "help") { printHelp(); return; }
    if (cmd == "exit") { std::cout << "Bye!\n"; exit(0); }

    // 进程管理
    if (cmd == "ps"     || cmd == "create" || cmd == "kill" ||
        cmd == "block"  || cmd == "wake"   || cmd == "run") {
        try { handleProc(args); } catch (...) { std::cout << "参数错误\n"; }
        return;
    }

    // 内存管理
    if (cmd == "memmap" || cmd == "malloc" || cmd == "mfree") {
        try { handleMem(args); } catch (...) { std::cout << "参数错误\n"; }
        return;
    }

    // 文件系统
    if (cmd == "pwd"   || cmd == "ls"    || cmd == "cd"    ||
        cmd == "mkdir" || cmd == "touch" || cmd == "write" ||
        cmd == "cat"   || cmd == "rm") {
        handleFS(args);
        return;
    }

    // 设备管理
    if (cmd == "devices" || cmd == "alloc" || cmd == "release") {
        try { handleDev(args); } catch (...) { std::cout << "参数错误\n"; }
        return;
    }

    std::cout << "未知命令: " << cmd << "  (输入 help 查看帮助)\n";
}

// ─────────────────────────────────────────────
// REPL 主循环
// ─────────────────────────────────────────────
void Shell::run() {
    std::cout << "欢迎使用 Yinix OS 模拟器  (输入 help 查看命令)\n";
    std::string line;
    while (true) {
        std::cout << "\nYinix> ";
        std::cout.flush();
        if (!std::getline(std::cin, line)) break;
        auto args = tokenize(line);
        dispatch(args);
    }
}
