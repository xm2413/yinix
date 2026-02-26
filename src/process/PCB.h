#pragma once
#include <string>

enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

struct PCB {
    int pid;
    std::string name;
    ProcessState state;
    int priority;
    int cpu_time;  // 已执行的 tick 数
};
