#include "DeviceManager.h"
#include <iostream>
#include <iomanip>

// 初始化固定设备池（课程演示用）。
DeviceManager::DeviceManager() {
    devices.emplace_back("printer0", "printer");
    devices.emplace_back("disk0",    "disk");
    devices.emplace_back("disk1",    "disk");
    devices.emplace_back("keyboard0","keyboard");
    devices.emplace_back("screen0",  "screen");
}

Device* DeviceManager::find(const std::string& name) {
    for (auto& d : devices)
        if (d.name == name) return &d;
    return nullptr;
}

bool DeviceManager::allocate(const std::string& name, int pid) {
    // 独占分配：设备忙时直接拒绝。
    Device* d = find(name);
    if (!d) { std::cout << "错误: 设备 '" << name << "' 不存在\n"; return false; }
    if (d->status == DeviceStatus::BUSY) {
        std::cout << "错误: 设备 '" << name << "' 正被进程 [" << d->ownerPid << "] 占用\n";
        return false;
    }
    d->status = DeviceStatus::BUSY;
    d->ownerPid = pid;
    std::cout << "设备 '" << name << "' 已分配给进程 [" << pid << "]\n";
    return true;
}

bool DeviceManager::release(const std::string& name) {
    Device* d = find(name);
    if (!d) { std::cout << "错误: 设备 '" << name << "' 不存在\n"; return false; }
    if (d->status == DeviceStatus::IDLE) { std::cout << "设备 '" << name << "' 本就空闲\n"; return false; }
    d->status = DeviceStatus::IDLE;
    d->ownerPid = -1;
    std::cout << "设备 '" << name << "' 已释放\n";
    return true;
}

void DeviceManager::listDevices() {
    std::cout << std::left
              << std::setw(14) << "设备名"
              << std::setw(12) << "类型"
              << std::setw(8)  << "状态"
              << "占用PID\n"
              << std::string(44, '-') << "\n";
    for (auto& d : devices) {
        std::cout << std::setw(14) << d.name
                  << std::setw(12) << d.type
                  << std::setw(8)  << (d.status == DeviceStatus::IDLE ? "空闲" : "占用")
                  << (d.status == DeviceStatus::IDLE ? "-" : std::to_string(d.ownerPid)) << "\n";
    }
}

void DeviceManager::releaseByPid(int pid) {
    // 进程退出后，回收其占用的全部设备。
    for (auto& d : devices) {
        if (d.ownerPid == pid) {
            d.status = DeviceStatus::IDLE;
            d.ownerPid = -1;
            std::cout << "设备 '" << d.name << "' 随进程 [" << pid << "] 退出自动释放\n";
        }
    }
}
