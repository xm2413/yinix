#pragma once
#include "FSNode.h"
#include <memory>
#include <string>

class FileSystem {
public:
    FileSystem();

    void pwd();
    void ls();
    std::string getCwd();  // 返回当前路径字符串，供 Shell 显示提示符
    bool cd(const std::string& path);
    bool mkdir(const std::string& name);
    bool touch(const std::string& name);
    bool write(const std::string& name, const std::string& content);
    bool read(const std::string& name);
    bool rm(const std::string& name);

private:
    std::shared_ptr<FSNode> root;
    FSNode* cwd;  // 当前工作目录

    FSNode* findChild(FSNode* dir, const std::string& name);
    std::string cwdPath();
};
