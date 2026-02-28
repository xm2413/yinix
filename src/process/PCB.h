#pragma once
#include <string>

// 进程状态机。
enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

// 进程控制块（Process Control Block）。
struct PCB {
    int pid;                // 进程唯一标识
    std::string name;       // 进程名
    ProcessState state;     // 当前状态
    int priority;           // 预留字段（当前调度未使用优先级）
    int cpu_time;           // 累计获得 CPU 的 tick 数
};
