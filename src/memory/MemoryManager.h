#pragma once
#include <list>
#include <string>

struct MemBlock {
    int start;
    int size;
    bool free;
    int ownerPid;  // -1 表示无主
};

class MemoryManager {
public:
    explicit MemoryManager(int totalSize = 1024);

    int  allocate(int size, int ownerPid = -1);  // 返回起始地址，-1 表示失败
    bool release(int addr);
    void releaseByPid(int pid);  // 进程退出时批量释放
    void printMap();

private:
    int total;
    std::list<MemBlock> blocks;

    void merge();  // 合并相邻空闲块
};
