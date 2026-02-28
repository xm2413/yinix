#include "SyncManager.h"
#include <iostream>
#include <iomanip>

SyncManager::SyncManager(ProcessManager& pm) : procMgr(pm) {}

Semaphore* SyncManager::find(const std::string& name) {
    auto it = semaphores.find(name);
    return (it == semaphores.end()) ? nullptr : &it->second;
}

bool SyncManager::create(const std::string& name, int initVal) {
    if (semaphores.count(name)) {
        std::cout << "错误: 信号量 '" << name << "' 已存在\n";
        return false;
    }
    Semaphore s;
    s.id    = nextId++;
    s.name  = name;
    s.value = initVal;
    semaphores[name] = std::move(s);
    std::cout << "信号量 '" << name << "' 已创建，初始值=" << initVal << "\n";
    return true;
}

bool SyncManager::destroy(const std::string& name) {
    auto it = semaphores.find(name);
    if (it == semaphores.end()) {
        std::cout << "错误: 信号量 '" << name << "' 不存在\n";
        return false;
    }
    semaphores.erase(it);
    std::cout << "信号量 '" << name << "' 已删除\n";
    return true;
}

bool SyncManager::P(const std::string& name, int pid) {
    Semaphore* s = find(name);
    if (!s) { std::cout << "错误: 信号量 '" << name << "' 不存在\n"; return false; }

    s->value--;
    std::cout << "P(" << name << ") by 进程[" << pid << "]，value=" << s->value << "\n";

    if (s->value < 0) {
        // 资源不足，进程进入该信号量的等待队列并阻塞。
        s->waitQueue.push(pid);
        std::cout << "  -> 进程[" << pid << "] 因资源不足被阻塞，加入等待队列\n";
        procMgr.blockProcess(pid);
    } else {
        std::cout << "  -> 进程[" << pid << "] 成功获得资源\n";
    }
    return true;
}

bool SyncManager::V(const std::string& name) {
    Semaphore* s = find(name);
    if (!s) { std::cout << "错误: 信号量 '" << name << "' 不存在\n"; return false; }

    s->value++;
    std::cout << "V(" << name << ")，value=" << s->value << "\n";

    if (s->value <= 0 && !s->waitQueue.empty()) {
        // 仍有等待进程，唤醒队头。
        int wakepid = s->waitQueue.front();
        s->waitQueue.pop();
        std::cout << "  -> 唤醒等待进程[" << wakepid << "]\n";
        procMgr.wakeProcess(wakepid);
    } else {
        std::cout << "  -> 无等待进程，资源已归还\n";
    }
    return true;
}

void SyncManager::list() {
    if (semaphores.empty()) {
        std::cout << "当前无信号量\n";
        return;
    }
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(16) << "名称"
              << std::setw(8)  << "当前值"
              << "等待进程数\n"
              << std::string(40, '-') << "\n";
    for (auto& [name, s] : semaphores) {
        // waitQueue 无 size()，用 value 负数代表等待数。
        int waiting = (s.value < 0) ? (-s.value) : 0;
        std::cout << std::setw(6)  << s.id
                  << std::setw(16) << s.name
                  << std::setw(8)  << s.value
                  << waiting << "\n";
    }
}
