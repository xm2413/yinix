#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ctime>

// 文件系统节点类型：文件/目录。
enum class FSNodeType { FILE, DIRECTORY };

// 目录树节点（文件与目录共用同一结构）。
struct FSNode {
    std::string name;                               // 节点名
    FSNodeType  type;                               // 节点类型
    std::string content;                            // 文件内容（目录忽略）
    time_t      ctime;                              // 创建时间
    std::vector<std::shared_ptr<FSNode>> children; // 子节点（仅目录使用）
    FSNode* parent = nullptr;                       // 父节点裸指针（不拥有）

    explicit FSNode(const std::string& n, FSNodeType t, FSNode* p = nullptr)
        : name(n), type(t), ctime(std::time(nullptr)), parent(p) {}
};
