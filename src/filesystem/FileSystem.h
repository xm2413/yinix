#pragma once
#include "FSNode.h"
#include <memory>
#include <string>
#include <fstream>

class FileSystem {
public:
    FileSystem();

    void pwd();
    void ls();
    std::string getCwd();
    bool cd(const std::string& path);
    bool mkdir(const std::string& name);
    bool touch(const std::string& name);
    bool write(const std::string& name, const std::string& content);
    bool read(const std::string& name);
    bool rm(const std::string& name);

    void save(const std::string& filePath);  // 序列化到文件
    void load(const std::string& filePath);  // 从文件反序列化

private:
    std::shared_ptr<FSNode> root;
    FSNode* cwd;

    FSNode* findChild(FSNode* dir, const std::string& name);
    std::string cwdPath();

    void saveNode(std::ofstream& out, FSNode* node, const std::string& path);
    static std::string escapeContent(const std::string& s);
    static std::string unescapeContent(const std::string& s);
    FSNode* mkdirP(const std::string& absPath);
};
