#include "MemoryManager.h"
#include <iostream>
#include <iomanip>

MemoryManager::MemoryManager(int totalSize) : total(totalSize) {
    blocks.push_back({0, totalSize, true, -1});
}

int MemoryManager::allocate(int size, int ownerPid) {
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        if (it->free && it->size >= size) {
            int addr = it->start;
            if (it->size > size) {
                // 切割剩余部分
                blocks.insert(std::next(it), {addr + size, it->size - size, true, -1});
            }
            it->size = size;
            it->free = false;
            it->ownerPid = ownerPid;
            std::cout << "内存分配成功: 地址=" << addr << " 大小=" << size << "\n";
            return addr;
        }
    }
    std::cout << "错误: 内存不足，无法分配 " << size << " 单位\n";
    return -1;
}

bool MemoryManager::release(int addr) {
    for (auto& b : blocks) {
        if (b.start == addr && !b.free) {
            b.free = true;
            b.ownerPid = -1;
            merge();
            std::cout << "内存释放成功: 地址=" << addr << "\n";
            return true;
        }
    }
    std::cout << "错误: 地址 " << addr << " 未被分配\n";
    return false;
}

void MemoryManager::releaseByPid(int pid) {
    bool any = false;
    for (auto& b : blocks) {
        if (!b.free && b.ownerPid == pid) {
            b.free = true;
            b.ownerPid = -1;
            any = true;
            std::cout << "内存块 [addr=" << b.start << ", size=" << b.size
                      << "] 随进程 [" << pid << "] 退出自动释放\n";
        }
    }
    if (any) merge();
}

void MemoryManager::merge() {
    auto it = blocks.begin();
    while (it != blocks.end()) {
        auto next = std::next(it);
        if (next != blocks.end() && it->free && next->free) {
            it->size += next->size;
            blocks.erase(next);
        } else {
            ++it;
        }
    }
}

void MemoryManager::printMap() {
    std::cout << "=== 内存布局 (共 " << total << " 单位) ===\n";
    std::cout << std::left << std::setw(8) << "起始" << std::setw(8) << "大小"
              << std::setw(8) << "状态" << "PID\n"
              << std::string(32, '-') << "\n";
    for (auto& b : blocks) {
        std::cout << std::setw(8) << b.start
                  << std::setw(8) << b.size
                  << std::setw(8) << (b.free ? "空闲" : "占用")
                  << (b.free ? "-" : std::to_string(b.ownerPid)) << "\n";
    }
}
