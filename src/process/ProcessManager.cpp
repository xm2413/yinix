#include "ProcessManager.h"
#include <iostream>
#include <iomanip>

ProcessManager::ProcessManager() {}

std::string ProcessManager::stateStr(ProcessState s) {
    switch (s) {
        case ProcessState::NEW:        return "NEW";
        case ProcessState::READY:      return "READY";
        case ProcessState::RUNNING:    return "RUNNING";
        case ProcessState::BLOCKED:    return "BLOCKED";
        case ProcessState::TERMINATED: return "TERMINATED";
    }
    return "?";
}

int ProcessManager::createProcess(const std::string& name, int priority) {
    PCB pcb{nextPid++, name, ProcessState::READY, priority, 0};
    table[pcb.pid] = pcb;
    readyQueue.push(pcb.pid);
    std::cout << "进程已创建: [" << pcb.pid << "] " << name << "\n";
    return pcb.pid;
}

bool ProcessManager::killProcess(int pid) {
    auto it = table.find(pid);
    if (it == table.end()) {
        std::cout << "错误: 进程 " << pid << " 不存在\n";
        return false;
    }
    it->second.state = ProcessState::TERMINATED;
    if (runningPid == pid) runningPid = -1;
    std::cout << "进程 [" << pid << "] 已终止\n";
    return true;
}

bool ProcessManager::blockProcess(int pid) {
    auto it = table.find(pid);
    if (it == table.end()) { std::cout << "错误: 进程 " << pid << " 不存在\n"; return false; }
    it->second.state = ProcessState::BLOCKED;
    if (runningPid == pid) runningPid = -1;
    std::cout << "进程 [" << pid << "] 已阻塞\n";
    return true;
}

bool ProcessManager::wakeProcess(int pid) {
    auto it = table.find(pid);
    if (it == table.end()) { std::cout << "错误: 进程 " << pid << " 不存在\n"; return false; }
    if (it->second.state != ProcessState::BLOCKED) {
        std::cout << "错误: 进程 " << pid << " 当前不是阻塞状态\n"; return false;
    }
    it->second.state = ProcessState::READY;
    readyQueue.push(pid);
    std::cout << "进程 [" << pid << "] 已唤醒，进入就绪队列\n";
    return true;
}

void ProcessManager::tick() {
    // 当前运行进程消耗一个 tick
    if (runningPid != -1 && table.count(runningPid)) {
        auto& cur = table[runningPid];
        if (cur.state == ProcessState::RUNNING) {
            cur.cpu_time++;
            cur.state = ProcessState::READY;
            readyQueue.push(runningPid);
        }
    }
    // 从就绪队列取下一个
    while (!readyQueue.empty()) {
        int pid = readyQueue.front(); readyQueue.pop();
        if (!table.count(pid)) continue;
        auto& next = table[pid];
        if (next.state == ProcessState::TERMINATED) continue;
        if (next.state == ProcessState::BLOCKED) continue;
        next.state = ProcessState::RUNNING;
        runningPid = pid;
        std::cout << "调度: 进程 [" << pid << "] " << next.name << " 获得 CPU\n";
        return;
    }
    runningPid = -1;
    std::cout << "就绪队列为空，CPU 空闲\n";
}

void ProcessManager::listProcesses() {
    std::cout << std::left
              << std::setw(6)  << "PID"
              << std::setw(16) << "名称"
              << std::setw(12) << "状态"
              << std::setw(8)  << "优先级"
              << "CPU时间\n"
              << std::string(50, '-') << "\n";
    for (auto& [pid, pcb] : table) {
        if (pcb.state == ProcessState::TERMINATED) continue;
        std::cout << std::setw(6)  << pid
                  << std::setw(16) << pcb.name
                  << std::setw(12) << stateStr(pcb.state)
                  << std::setw(8)  << pcb.priority
                  << pcb.cpu_time << "\n";
    }
}
