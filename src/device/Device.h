#pragma once
#include <string>

enum class DeviceStatus { IDLE, BUSY };

struct Device {
    std::string  name;
    std::string  type;        // 如 "printer", "disk", "keyboard"
    DeviceStatus status;
    int          ownerPid;   // -1 表示空闲

    Device(const std::string& n, const std::string& t)
        : name(n), type(t), status(DeviceStatus::IDLE), ownerPid(-1) {}
};
