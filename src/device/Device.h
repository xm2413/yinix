#pragma once
#include <string>

// 设备状态：空闲/占用。
enum class DeviceStatus { IDLE, BUSY };

// 虚拟设备描述。
struct Device {
    std::string  name;        // 设备实例名（如 printer0）
    std::string  type;        // 设备类型（printer/disk/...）
    DeviceStatus status;      // 当前状态
    int          ownerPid;    // 当前占用该设备的进程 PID，-1 表示空闲

    Device(const std::string& n, const std::string& t)
        : name(n), type(t), status(DeviceStatus::IDLE), ownerPid(-1) {}
};
