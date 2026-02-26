#include "Shell.h"
#include <iostream>
#include <sstream>

Shell::Shell() {}

std::vector<std::string> Shell::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

void Shell::printHelp() {
    std::cout << "=== Yinix Help ===\n"
              << "  help          显示帮助\n"
              << "  exit          退出\n"
              << "  [模块命令待实现]\n";
}

void Shell::dispatch(const std::vector<std::string>& args) {
    if (args.empty()) return;
    const std::string& cmd = args[0];

    if (cmd == "help") {
        printHelp();
    } else if (cmd == "exit") {
        std::cout << "Bye!\n";
        exit(0);
    } else {
        std::cout << "未知命令: " << cmd << "  (输入 help 查看帮助)\n";
    }
}

void Shell::run() {
    std::string line;
    while (true) {
        std::cout << "Yinix> ";
        std::cout.flush();
        if (!std::getline(std::cin, line)) break;
        auto args = tokenize(line);
        dispatch(args);
    }
}
