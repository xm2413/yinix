#pragma once
#include "PCB.h"
#include <map>
#include <queue>
#include <vector>

// 进程管理器：维护进程表、就绪队列，并提供 RR 调度。
class ProcessManager {
public:
    ProcessManager();

    // 创建进程并进入就绪队列，返回 PID。
    int  createProcess(const std::string& name, int priority = 0);

    // 终止/阻塞/唤醒指定 PID，成功返回 true。
    bool killProcess(int pid);
    bool blockProcess(int pid);
    bool wakeProcess(int pid);

    // 触发一次时间片轮转调度。
    void tick();           // 时间片轮转调度一次

    // 打印当前非终止进程表。
    void listProcesses(); // 打印进程表

private:
    // PID -> PCB。
    std::map<int, PCB> table;

    // 就绪队列（FIFO）。
    std::queue<int> readyQueue;

    // PID 分配器与当前运行进程。
    int nextPid = 1;
    int runningPid = -1;  // 当前运行的 PID，-1 表示无

    // 状态枚举转文本。
    std::string stateStr(ProcessState s);
};
