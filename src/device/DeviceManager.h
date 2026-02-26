#pragma once
#include "Device.h"
#include <vector>

class DeviceManager {
public:
    DeviceManager();

    bool allocate(const std::string& name, int pid);
    bool release(const std::string& name);
    void listDevices();
    void releaseByPid(int pid);  // 进程退出时批量释放

private:
    std::vector<Device> devices;
    Device* find(const std::string& name);
};
