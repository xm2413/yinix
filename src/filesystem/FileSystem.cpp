#include "FileSystem.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ctime>

FileSystem::FileSystem() {
    root = std::make_shared<FSNode>("/", FSNodeType::DIRECTORY, nullptr);
    cwd = root.get();
}

FSNode* FileSystem::findChild(FSNode* dir, const std::string& name) {
    for (auto& c : dir->children)
        if (c->name == name) return c.get();
    return nullptr;
}

std::string FileSystem::cwdPath() {
    if (cwd == root.get()) return "/";
    std::string path;
    FSNode* cur = cwd;
    while (cur && cur != root.get()) {
        path = "/" + cur->name + path;
        cur = cur->parent;
    }
    return path;
}

void FileSystem::pwd() {
    std::cout << cwdPath() << "\n";
}

void FileSystem::ls() {
    std::cout << "目录: " << cwdPath() << "\n";
    for (auto& c : cwd->children) {
        std::string type = (c->type == FSNodeType::DIRECTORY) ? "[DIR] " : "[FILE]";
        char buf[32];
        std::strftime(buf, sizeof(buf), "%m-%d %H:%M", std::localtime(&c->ctime));
        std::cout << "  " << type << "  " << std::left << std::setw(20) << c->name << buf << "\n";
    }
}

bool FileSystem::cd(const std::string& path) {
    if (path == "/") { cwd = root.get(); return true; }
    if (path == "..") {
        if (cwd->parent) cwd = cwd->parent;
        return true;
    }
    FSNode* target = findChild(cwd, path);
    if (!target) { std::cout << "错误: 目录 '" << path << "' 不存在\n"; return false; }
    if (target->type != FSNodeType::DIRECTORY) { std::cout << "错误: '" << path << "' 不是目录\n"; return false; }
    cwd = target;
    return true;
}

bool FileSystem::mkdir(const std::string& name) {
    if (findChild(cwd, name)) { std::cout << "错误: '" << name << "' 已存在\n"; return false; }
    cwd->children.push_back(std::make_shared<FSNode>(name, FSNodeType::DIRECTORY, cwd));
    std::cout << "目录 '" << name << "' 已创建\n";
    return true;
}

bool FileSystem::touch(const std::string& name) {
    if (findChild(cwd, name)) { std::cout << "错误: '" << name << "' 已存在\n"; return false; }
    cwd->children.push_back(std::make_shared<FSNode>(name, FSNodeType::FILE, cwd));
    std::cout << "文件 '" << name << "' 已创建\n";
    return true;
}

bool FileSystem::write(const std::string& name, const std::string& content) {
    FSNode* node = findChild(cwd, name);
    if (!node) { std::cout << "错误: 文件 '" << name << "' 不存在\n"; return false; }
    if (node->type != FSNodeType::FILE) { std::cout << "错误: '" << name << "' 不是文件\n"; return false; }
    node->content = content;
    std::cout << "写入成功\n";
    return true;
}

bool FileSystem::read(const std::string& name) {
    FSNode* node = findChild(cwd, name);
    if (!node) { std::cout << "错误: 文件 '" << name << "' 不存在\n"; return false; }
    if (node->type != FSNodeType::FILE) { std::cout << "错误: '" << name << "' 不是文件\n"; return false; }
    std::cout << node->content << "\n";
    return true;
}

bool FileSystem::rm(const std::string& name) {
    auto& ch = cwd->children;
    auto it = std::find_if(ch.begin(), ch.end(),
        [&](const std::shared_ptr<FSNode>& n){ return n->name == name; });
    if (it == ch.end()) { std::cout << "错误: '" << name << "' 不存在\n"; return false; }
    ch.erase(it);
    std::cout << "'" << name << "' 已删除\n";
    return true;
}
