#pragma once
#include "FSNode.h"
#include <memory>
#include <string>
#include <fstream>

// 进程内文件系统：目录树 + 当前工作目录 + 简单持久化。
class FileSystem {
public:
    FileSystem();

    // 与 Shell 命令一一对应的文件系统操作。
    void pwd();
    void ls();
    std::string getCwd();
    bool cd(const std::string& path);
    bool mkdir(const std::string& name);
    bool touch(const std::string& name);
    bool write(const std::string& name, const std::string& content);
    bool read(const std::string& name);
    bool rm(const std::string& name);

    // 将目录树保存/恢复到磁盘文件。
    void save(const std::string& filePath);  // 序列化到文件
    void load(const std::string& filePath);  // 从文件反序列化

private:
    // 根目录与当前目录。
    std::shared_ptr<FSNode> root;
    FSNode* cwd;

    // 在目录下查找子节点。
    FSNode* findChild(FSNode* dir, const std::string& name);

    // 获取当前目录绝对路径。
    std::string cwdPath();

    // 深度优先序列化整棵子树。
    void saveNode(std::ofstream& out, FSNode* node, const std::string& path);

    // 文件内容转义/反转义（用于文本持久化格式）。
    static std::string escapeContent(const std::string& s);
    static std::string unescapeContent(const std::string& s);

    // 按绝对路径逐级创建目录（mkdir -p 语义）。
    FSNode* mkdirP(const std::string& absPath);
};
