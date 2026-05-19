# 🚀 Vertical Platform Coordinated Control System (VPCCS)

![License](https://img.shields.io/badge/License-Proprietary-blue.svg)
![Qt](https://img.shields.io/badge/Qt-5.14+-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![Hardware](https://img.shields.io/badge/Hardware-PCI5565_RFM-orange.svg)
![Language](https://img.shields.io/badge/Language-C++-purple.svg)

**VPCCS** is an advanced intelligent Human-Machine Interface (HMI) control software developed for aerospace electromechanical equipment. Built with modern `C++/Qt`, this software deeply integrates the VMIPMC-5565 (PCI5565) Reflective Memory card to achieve microsecond-level, deterministic, and lock-free real-time data exchange with the low-level control systems.

---

## ✨ Key Features

- 🖥️ **Panoramic Visualization & HMI**
  Leverages the Qt framework and `QCustomPlot` to provide a high-refresh-rate dashboard monitoring the 6-axis robotic arm (`Theta1~6`), end-effector spatial pose (`X/Y/Z, Roll/Pitch/Yaw`), velocity, and torque in real-time.
- ⚡ **Microsecond Deterministic Communication**
  Fully integrated with **PCI5565 Reflective Memory** hardware via `RFM2g API`, establishing a zero-latency data pipeline that meets strict aerospace-grade real-time control requirements.
- 🔄 **Lock-free High Concurrency Architecture**
  Implements a decoupled multi-threading architecture (separated RFM data acquisition, asynchronous logging, and UI rendering). Utilizes double-buffering and atomic indexing techniques to ensure UI fluidity even under massive data throughput.
- 🛡️ **Multi-dimensional Safety Monitoring**
  Features a built-in safety interface that synergizes with low-level sensor data to evaluate spatial interference models. Delivers real-time collision warnings and hardware error codes to prevent equipment damage.

---

## 📂 Project Structure

```text
📦 518QTcode
 ┣ 📂 Icon/                  # UI assets and icons
 ┣ 📜 Qt_UDP.pro             # Qt project configuration file
 ┣ 📜 mainwindow.cpp/.h      # Main dashboard and core control logic
 ┣ 📜 mechanical_arm.cpp/.h  # Dedicated robotic arm parameter monitoring
 ┣ 📜 qcustomplot.cpp/.h     # High-performance plotting library
 ┣ 📜 receive.cpp/.h         # Motion state data parsing module
 ┣ 📜 work.cpp/.h            # RFM data polling and timer threads
 ┣ 📜 qlog.cpp/.h            # Asynchronous persistent logging
 ┣ 📜 excelreader.cpp/.h     # External data I/O and reporting module
 ┗ 📜 pmc5565_nt.h / rfm2g*  # PCI5565 Reflective Memory hardware drivers
```

---

## 🛠️ Build & Installation

### 1. Prerequisites
- **IDE**: Qt Creator
- **Framework**: Qt 5.14 or newer (Qt 6 compatible)
- **Compiler**: MinGW / MSVC (Must match the architecture of the RFM `*.lib` files)
- **Hardware**: Windows environment with a physical **VMIPMC-5565 (PCI5565)** card and its drivers installed. RFM2g API initialization will fail without the hardware.

### 2. Quick Start
1. Clone the repository:
   ```bash
   git clone https://github.com/Lvmingxingbwh/tiang-518qt.git
   ```
2. Open `Qt_UDP.pro` in Qt Creator.
3. Configure your Build Kit (64-bit recommended).
4. Verify the RFM library paths (`rfm2gdll_stdc_64.lib`) in the `.pro` file.
5. Click **Build and Run**.

---

## ⚠️ Important Notes

- **Offline Mode**: As this is a customized control system for specific aerospace hardware, running the application without the physical Reflective Memory network will trigger offline warnings and display default fallback values.
- **Logging**: Ensure the application has write permissions in its execution directory, as the asynchronous logging system will auto-generate a `logs/` directory.
