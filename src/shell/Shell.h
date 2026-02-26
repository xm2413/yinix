#pragma once
#include <string>
#include <vector>

class Shell {
public:
    Shell();
    void run();  // 启动主循环

private:
    void dispatch(const std::vector<std::string>& args);
    void printHelp();

    std::vector<std::string> tokenize(const std::string& line);
};
