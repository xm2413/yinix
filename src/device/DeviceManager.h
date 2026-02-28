#pragma once
#include "Device.h"
#include <vector>

// 设备管理器：维护设备表，提供独占分配与释放。
class DeviceManager {
public:
    DeviceManager();

    // 分配设备给进程（独占），成功返回 true。
    bool allocate(const std::string& name, int pid);

    // 释放设备，成功返回 true。
    bool release(const std::string& name);

    // 打印设备状态表。
    void listDevices();

    // 按 PID 批量释放设备（进程退出联动）。
    void releaseByPid(int pid);  // 进程退出时批量释放

private:
    // 所有预置设备。
    std::vector<Device> devices;

    // 按名称查找设备，找不到返回 nullptr。
    Device* find(const std::string& name);
};
