#include "FileSystem.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <fstream>

// 序列化辅助：将内容转为单行安全文本。
// 转义规则：\ -> \\，换行 -> \n，分隔符 | -> \|
std::string FileSystem::escapeContent(const std::string& s) {
    std::string r;
    for (char c : s) {
        if      (c == '\\') r += "\\\\";
        else if (c == '\n') r += "\\n";
        else if (c == '|')  r += "\\|";
        else                r += c;
    }
    return r;
}

std::string FileSystem::unescapeContent(const std::string& s) {
    std::string r;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            ++i;
            if      (s[i] == 'n')  r += '\n';
            else if (s[i] == '|')  r += '|';
            else                   r += s[i];
        } else {
            r += s[i];
        }
    }
    return r;
}

// 深度优先写入节点。
// 持久化格式：D|/path   或   F|/path|escaped_content
void FileSystem::saveNode(std::ofstream& out, FSNode* node, const std::string& path) {
    if (node->type == FSNodeType::DIRECTORY) {
        out << "D|" << path << "\n";
        for (auto& child : node->children) {
            std::string childPath = (path == "/" ? "" : path) + "/" + child->name;
            saveNode(out, child.get(), childPath);
        }
    } else {
        out << "F|" << path << "|" << escapeContent(node->content) << "\n";
    }
}

void FileSystem::save(const std::string& filePath) {
    // 只保存根目录下的真实子节点，根本身无需落盘。
    std::ofstream out(filePath);
    if (!out) { std::cerr << "[warn] 无法保存文件系统到 " << filePath << "\n"; return; }
    for (auto& child : root->children)
        saveNode(out, child.get(), "/" + child->name);
}

// 按绝对路径逐级创建目录（类似 mkdir -p）。
FSNode* FileSystem::mkdirP(const std::string& absPath) {
    FSNode* cur = root.get();
    std::istringstream ss(absPath);
    std::string seg;
    while (std::getline(ss, seg, '/')) {
        if (seg.empty()) continue;
        FSNode* child = findChild(cur, seg);
        if (!child) {
            cur->children.push_back(std::make_shared<FSNode>(seg, FSNodeType::DIRECTORY, cur));
            child = cur->children.back().get();
        }
        cur = child;
    }
    return cur;
}

void FileSystem::load(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in) return;  // 首次运行，文件不存在属正常

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find('|');
        if (p1 == std::string::npos) continue;
        std::string type = line.substr(0, p1);
        std::string rest = line.substr(p1 + 1);

        if (type == "D") {
            // 目录记录：直接补齐路径。
            mkdirP(rest);
        } else if (type == "F") {
            // 文件记录：拆分出“路径 + 内容”。
            size_t p2 = rest.find('|');
            std::string path    = (p2 == std::string::npos) ? rest : rest.substr(0, p2);
            std::string content = (p2 == std::string::npos) ? ""   : unescapeContent(rest.substr(p2 + 1));

            size_t slash = path.rfind('/');
            std::string parentPath = (slash == 0) ? "/" : path.substr(0, slash);
            std::string name       = path.substr(slash + 1);

            // 父目录不存在时自动补齐，再创建文件节点。
            FSNode* parent = (parentPath == "/") ? root.get() : mkdirP(parentPath);
            if (!findChild(parent, name)) {
                auto node = std::make_shared<FSNode>(name, FSNodeType::FILE, parent);
                node->content = content;
                parent->children.push_back(std::move(node));
            }
        }
    }
}

// ---- 运行时文件系统操作（对应 Shell 命令） ----
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
    // 由 cwd 沿 parent 回溯生成绝对路径。
    if (cwd == root.get()) return "/";
    std::string path;
    FSNode* cur = cwd;
    while (cur && cur != root.get()) {
        path = "/" + cur->name + path;
        cur = cur->parent;
    }
    return path;
}

std::string FileSystem::getCwd() {
    return cwdPath();
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
    // 支持根目录、父目录和当前目录下子目录三类路径。
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
    // 直接从当前目录 children 删除目标节点（共享指针自动回收子树）。
    auto& ch = cwd->children;
    auto it = std::find_if(ch.begin(), ch.end(),
        [&](const std::shared_ptr<FSNode>& n){ return n->name == name; });
    if (it == ch.end()) { std::cout << "错误: '" << name << "' 不存在\n"; return false; }
    ch.erase(it);
    std::cout << "'" << name << "' 已删除\n";
    return true;
}
