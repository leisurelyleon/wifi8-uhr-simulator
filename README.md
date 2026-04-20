# 📡 IEEE 802.11bn (Wi-Fi 8) UHR Engine

![Status](https://img.shields.io/badge/Status-Predictive_Simulation-0ea5e9?style=for-the-badge)
![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B)
![React](https://img.shields.io/badge/React-18-20232a?style=for-the-badge&logo=react)
![TypeScript](https://img.shields.io/badge/TypeScript-5.0-3178c6?style=for-the-badge&logo=typescript)

> **⚠️ DISCLAIMER: PURELY PREDICTIVE CONCEPTUAL PROJECT** > This repository contains **no leaked code, proprietary information, or actual IEEE draft specifications**. It is a purely predictive programming exercise designed to conceptualize and simulate the expected logic of the upcoming Wi-Fi 8 (802.11bn) Ultra-High Reliability (UHR) standard anticipated for 2028.

## 🌌 The Vision
The **Wi-Fi 8 UHR Engine** is a full-stack architectural simulation exploring the shift from raw throughput to absolute, deterministic reliability. This project bridges a high-performance C++ backend simulation engine with a modern, high-transparency "Glass Oasis" aesthetic React dashboard to visualize network states in real-time.

## 🚀 Core Predictive Features

* **Multi-Access Point Coordination (MAPC):** Simulates Joint Transmission (JT) logic, dynamically synchronizing multiple Access Points to deliver packets to a client simultaneously with microsecond precision.
* **Precognitive ML Interference Engine:** Replaces reactive packet-loss detection with a simulated lightweight 1D-Convolutional model to predict RF jitter *before* it happens, dynamically resizing buffers in real-time.
* **Sub-THz Hardware Bridging:** Models zero-latency fallback protocols between experimental 120GHz Line-of-Sight links and robust 6GHz baselines.
* **Coordinated Spatial Reuse (CSR):** Dynamically adjusts BSS Colors and clamping TX power to navigate Overlapping Basic Service Sets (OBSS).

## 📂 Architecture Structure

```text
wifi8-uhr-simulator/
├── engine/                         # C++ Core Simulation Engine
│   ├── src/
│   │   ├── ap_coordination/        # Logic for Multi-AP synchronized packet delivery
│   │   │   ├── MapcController.cpp
│   │   │   └── SpatialReuse.cpp
│   │   ├── ml_predictor/           # Predictive jitter & interference modeling
│   │   │   ├── InterferenceEngine.cpp
│   │   │   └── JitterBuffer.cpp
│   │   ├── phy_mac/                # Simulated 802.11bn MAC layer handling
│   │   │   ├── SubThzBridge.cpp
│   │   │   └── FrameAggregator.cpp
│   │   └── main.cpp                # Engine entry point and WebSocket server
│   ├── include/                    # C++ Header files
│   │   ├── ap_coordination.hpp
│   │   ├── ml_predictor.hpp
│   │   └── phy_mac.hpp
│   └── CMakeLists.txt              # C++ Build configuration
├── dashboard/                      # Web Visualization Dashboard (TS/React)
│   ├── src/
│   │   ├── components/
│   │   │   ├── NetworkGraph.tsx    # Visualizes AP nodes and client movement
│   │   │   ├── PacketLossChart.tsx # Real-time telemetry chart
│   │   │   └── ControlPanel.tsx    # Toggles to inject interference
│   │   ├── services/
│   │   │   └── telemetrySocket.ts  # WebSocket listener for engine data
│   │   ├── utils/
│   │   │   └── packetParsers.ts
│   │   ├── App.tsx
│   │   └── index.css
│   ├── package.json
│   └── tsconfig.json
├── tests/                          # Automated Testing
│   ├── engine_tests/               # C++ Catch2/GTest files
│   └── dashboard_tests/            # Jest/React Testing Library files
├── docs/
│   ├── architecture_flow.md        # Mermaid.js flowcharts of the mapc process
│   └── predictive_models.md        # Explanations of the ML theory used
├── .gitignore
└── README.md                       # The master project manifesto
```

## 🛠️ Getting Started

### 1. Booting the C++ Engine

Ensure you have CMake (3.14+) and a C++17 compliant compiler installed.

```bash
cd engine
mkdir build && cd build
cmake ..
cmake --build . --config Release
./wifi8_engine
```

### 2. Launching the NOC Dashboard

Ensure you have Node.js installed.

```bash
cd dashboard
npm install
npm run dev
```

## 📊 Live Telemetry

Once both systems are running, the C++ engine broadcasts real-time telemetry via WebSocket to the React frontend. Users can dynamically inject RF interference or simulate physical Line-of-Sight blockages to watch the network adapt autonomously.

---

*Conceptualized and built as an advanced technical study in network infrastructure and multi-threaded systems.*
