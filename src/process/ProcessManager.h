#pragma once
#include "PCB.h"
#include <map>
#include <queue>
#include <vector>

class ProcessManager {
public:
    ProcessManager();

    int  createProcess(const std::string& name, int priority = 0);
    bool killProcess(int pid);
    bool blockProcess(int pid);
    bool wakeProcess(int pid);
    void tick();           // 时间片轮转调度一次
    void listProcesses(); // 打印进程表

private:
    std::map<int, PCB> table;
    std::queue<int> readyQueue;
    int nextPid = 1;
    int runningPid = -1;  // 当前运行的 PID，-1 表示无

    std::string stateStr(ProcessState s);
};
