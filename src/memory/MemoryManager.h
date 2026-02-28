#pragma once
#include <list>
#include <string>

// 内存块描述：用于模拟分区式内存管理。
struct MemBlock {
    int start;      // 起始地址
    int size;       // 块大小
    bool free;      // 是否空闲
    int ownerPid;   // 占用该块的进程 PID，-1 表示无主
};

// 内存管理器：First Fit 分配 + 相邻空闲块合并。
class MemoryManager {
public:
    explicit MemoryManager(int totalSize = 1024);

    // 分配指定大小内存，成功返回起始地址，失败返回 -1。
    int  allocate(int size, int ownerPid = -1);  // 返回起始地址，-1 表示失败

    // 按起始地址释放内存块。
    bool release(int addr);

    // 按 PID 批量释放（用于进程退出联动回收）。
    void releaseByPid(int pid);  // 进程退出时批量释放

    // 打印当前内存布局。
    void printMap();

private:
    int total;                   // 模拟总内存容量
    std::list<MemBlock> blocks;  // 按地址有序的内存块链表

    // 合并相邻空闲块，减少外部碎片。
    void merge();  // 合并相邻空闲块
};
