# Yinix 测试套件

## 结构

```
tests/
├── helper.sh           # 通用断言工具函数
├── test_process.sh     # 进程管理 9 个用例
├── test_memory.sh      # 内存管理 8 个用例
├── test_filesystem.sh  # 文件系统 12 个用例
├── test_device.sh      # 设备管理 7 个用例
├── test_integration.sh # 集成联动 6 个用例
└── run_all.sh          # 一键运行全部
```

## 运行方式

```bash
cd tests
bash run_all.sh
```

## 用例说明

| 文件 | 模块 | 用例数 | 覆盖内容 |
|---|---|---|---|
| `test_process.sh` | 进程管理 | 9 | 创建、列表、状态转换、调度、错误处理 |
| `test_memory.sh` | 内存管理 | 8 | 分配、释放、空闲合并、溢出、错误处理 |
| `test_filesystem.sh` | 文件系统 | 12 | 目录导航、文件增删读写、错误处理 |
| `test_device.sh` | 设备管理 | 7 | 设备列表、分配、释放、冲突检测 |
| `test_integration.sh` | 集成联动 | 6 | kill 进程自动释放内存与设备 |

**合计：42 个用例**
