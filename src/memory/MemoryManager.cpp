#include "MemoryManager.h"
#include <iostream>
#include <iomanip>

// 初始化为一个完整空闲块。
MemoryManager::MemoryManager(int totalSize) : total(totalSize) {
    blocks.push_back({0, totalSize, true, -1});
}

int MemoryManager::allocate(int size, int ownerPid) {
    // First Fit：从低地址到高地址找第一个可容纳块。
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        if (it->free && it->size >= size) {
            int addr = it->start;
            if (it->size > size) {
                // 若块更大，切割出“已分配块 + 剩余空闲块”。
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
    // 按起始地址精确匹配释放，避免误释放。
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
    // 扫描并回收属于该 PID 的全部内存块。
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
    // 链表天然保持地址顺序，线性合并相邻空闲块即可。
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
