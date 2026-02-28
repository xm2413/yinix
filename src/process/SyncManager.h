#pragma once
#include "Semaphore.h"
#include "ProcessManager.h"
#include <map>
#include <string>

// 同步管理器：以经典 PV 信号量为核心，实现进程同步与互斥。
// 依赖 ProcessManager 完成进程的阻塞与唤醒。
class SyncManager {
public:
    // 传入 ProcessManager 引用，以便 P/V 操作直接阻塞/唤醒进程。
    explicit SyncManager(ProcessManager& pm);

    // 创建信号量，初始值为 initVal。成功返回 true。
    bool create(const std::string& name, int initVal);

    // 删除信号量。成功返回 true。
    bool destroy(const std::string& name);

    // P 操作（Wait）：value--；若 value < 0，则将 pid 放入等待队列并阻塞。
    bool P(const std::string& name, int pid);

    // V 操作（Signal）：value++；若 value <= 0，则从等待队列唤醒一个进程。
    bool V(const std::string& name);

    // 打印所有信号量状态。
    void list();

private:
    ProcessManager& procMgr;
    std::map<std::string, Semaphore> semaphores;
    int nextId = 1;

    // 按名称查找，不存在返回 nullptr。
    Semaphore* find(const std::string& name);
};
