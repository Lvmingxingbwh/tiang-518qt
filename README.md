# 🚀 竖向平动协同控制系统上位机软件 (Vertical Platform Coordinated Control System)

![License](https://img.shields.io/badge/License-Proprietary-blue.svg)
![Qt](https://img.shields.io/badge/Qt-5.14+-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![Hardware](https://img.shields.io/badge/Hardware-PCI5565_RFM-orange.svg)

本项目为航天机电设备研究项目——**竖向平动协同控制系统**的智能上位机人机交互软件。基于 `C++/Qt` 开发，深度集成 VMIPMC-5565 (PCI5565) 反射内存卡底层驱动，实现与底层控制系统微秒级、高确定性的海量数据实时交互。

---

## ✨ 核心特性 (Key Features)

- 🖥️ **全景式人机交互与可视化**
  采用 Qt 与 `QCustomPlot` 框架，实时高刷显示机械臂的六轴角度 (`Theta1~6`)、末端位姿 (`X/Y/Z, Roll/Pitch/Yaw`)、运行速度及扭矩状态，实现全局无死角监控。
- ⚡ **微秒级高频确定性通讯架构**
  打通 **PCI5565 反射内存 (Reflective Memory)** 与 `RFM2g API`，构建了零延迟的数据链路，满足严苛的航天级实时控制需求。
- 🔄 **无锁化高并发处理设计**
  设计分离式多线程架构（网络/RFM接收、日志处理、UI刷新独立），结合双缓冲与原子化索引切换理念，保障海量数据吞吐时的 UI 极致流畅。
- 🛡️ **多维安全预警监控**
  预留安全监控与空间干涉报警接口，配合底层传感数据，在界面端实时输出碰撞预警与设备错误码，全方位保障设备运行安全。

---

## 📂 项目结构 (Project Structure)

```text
📦 518QTcode
 ┣ 📂 Icon/                  # UI 界面图标与相关资源
 ┣ 📜 Qt_UDP.pro             # Qt 项目工程配置
 ┣ 📜 mainwindow.cpp/.h      # 主交互界面与核心控制逻辑
 ┣ 📜 mechanical_arm.cpp/.h  # 机械臂参数专项监控界面
 ┣ 📜 qcustomplot.cpp/.h     # 高性能实时图表绘制库
 ┣ 📜 receive.cpp/.h         # 运动状态数据解析模块
 ┣ 📜 work.cpp/.h            # 定时轮询与反射内存卡 (RFM) 数据读取线程
 ┣ 📜 qlog.cpp/.h            # 异步日志持久化记录
 ┣ 📜 excelreader.cpp/.h     # 报表与外部数据导入模块
 ┗ 📜 pmc5565_nt.h / rfm2g*  # PCI5565 反射内存卡硬件驱动头文件与依赖库
```

---

## 🛠️ 环境依赖与编译构建 (Build Setup)

### 1. 软件依赖
- **IDE**: Qt Creator
- **Framework**: Qt 5.14 或更高版本 (兼容 Qt 6)
- **Compiler**: MinGW / MSVC (需匹配对应位数的 RFM `*.lib` 驱动库)

### 2. 硬件依赖
- Windows 运行环境。
- 必须安装 **VMIPMC-5565 (PCI5565)** 硬件设备及对应的 Windows 驱动程序，否则 RFM2g API 初始化会失败。

### 3. 构建步骤
1. 克隆本仓库到本地环境：
   ```bash
   git clone https://github.com/Lvmingxingbwh/tiang-518qt.git
   ```
2. 使用 Qt Creator 打开 `Qt_UDP.pro`。
3. 在 `Projects` 中配置您的构建套件 (推荐使用 64-bit)。
4. 点击 `Build` 编译工程。

---

## ⚠️ 注意事项 (Notes)

- 编译前请务必确认 `Qt_UDP.pro` 中的动态/静态链接库（如 `rfm2gdll_stdc_64.lib`）路径配置正确。
- 日志系统会自动在运行目录下生成 `logs/` 文件夹并异步写入，请确保有相应的磁盘写入权限。

---

> **Note**: 本项目为特定航天装备定制研发的配套控制软件，未连接真实下位机与反射内存网络时，部分状态可能会显示默认值或抛出离线警告。
