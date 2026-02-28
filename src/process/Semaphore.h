#pragma once
#include <string>
#include <queue>

// 信号量：用于进程间同步与互斥。
// value > 0：可用资源数。
// value = 0：无剩余资源，下一个 P 操作将阻塞。
// value < 0：|value| 表示当前等待中的进程数。
struct Semaphore {
    int         id;         // 唯一标识
    std::string name;       // 可读名称
    int         value;      // 当前值
    std::queue<int> waitQueue;  // 阻塞在此信号量上的进程 PID 队列（FIFO）
};
