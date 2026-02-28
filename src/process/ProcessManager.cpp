#include "ProcessManager.h"
#include <iostream>
#include <iomanip>

// 无额外资源初始化，保持默认构造即可。
ProcessManager::ProcessManager() {}

// 将进程状态转换为表格展示字符串。
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
    // 新建进程直接进入 READY，并入队等待调度。
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
    // 若正在运行该进程，CPU 立刻置空，等待下一次 tick 选新进程。
    if (runningPid == pid) runningPid = -1;
    std::cout << "进程 [" << pid << "] 已终止\n";
    return true;
}

bool ProcessManager::blockProcess(int pid) {
    auto it = table.find(pid);
    if (it == table.end()) { std::cout << "错误: 进程 " << pid << " 不存在\n"; return false; }
    it->second.state = ProcessState::BLOCKED;
    // 运行中的进程被阻塞时，应立即让出 CPU。
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
    // 唤醒后回到 READY，并重新进入就绪队列。
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
    // 从就绪队列取下一个可运行进程（跳过无效/已终止/阻塞项）
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
    // 仅展示活跃进程，终止进程默认不显示。
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
