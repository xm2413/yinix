#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ctime>

enum class FSNodeType { FILE, DIRECTORY };

struct FSNode {
    std::string name;
    FSNodeType  type;
    std::string content;   // 仅文件使用
    time_t      ctime;
    std::vector<std::shared_ptr<FSNode>> children;
    FSNode* parent = nullptr;

    explicit FSNode(const std::string& n, FSNodeType t, FSNode* p = nullptr)
        : name(n), type(t), ctime(std::time(nullptr)), parent(p) {}
};
