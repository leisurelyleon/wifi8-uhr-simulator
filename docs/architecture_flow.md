# IEEE 802.11bn (Wi-FI 8) UHR System Architecture

This document outlines the overarching data flow and component interactions within the Ultra-High Reliability (UHR) Multi-Access Point Coordination (MAPC) Engine. The architecture is split between a high-perfomrance C++ backend and a real-time TypeScript/React visualization dashboard.

## Core System Topology

The following diagram illustrates the lifestyle of a packet as it is evaluated, syncrhonized, and dispatched through the UHR ecosystem.

```mermaid
graph TD
    %% Define Styles
    classDef engine fill:#0f172a,stroke:#38bdf8,stroke-width:2px,color:#fff;
    classDef hardware fill:#1e293b,stroke:#a855f7,stroke-width:2px,color:#fff;
    classDef frontend fill:#1e1e1e,stroke:#10b981,stroke-width:2px,color:#fff;

    %% Components
    A[Enviornment Telemetry] -->|RF Noise, Utilization| B(ML Interference Engine)
    B -->|Predicted Jitter Delay| C{Jitter Buffer Allocation|

    C -->|Normal Ops| D(MAPC Controller)
    C -->|High Jitter| E(Dynamic Buffer Expansion)
    E --> D

    D -->|Synchronization Matrix| F[Spatial Reuse Engine]
    F -->|BSS Color Shift / Power Tune| G(PHY/MAC Layer)

    G --> H{Line of Sight Status}
    H -->|Clear| I[Sub-THz Bridge 120GHz]
    H -->|Blocked| J[Fallback 6GHz Link]

    I --> K((Joint Transmission to Client))
    J --> K((Joint Transmission to Client))

    %% Dashboard Flow
    B -.->|WebSocket JSON Payload| L[React Dashboard]
    G _._>|Link Status Payload| L

    %% Apply Styles
    class B,C,D,E,F engine;
    class G,H,I,J hardware;
    class L frontend;
```

# Component Breakdown

## 1. The Machine Learning Precognition Layer (ml_predictor)

Instead of reacting to packet loss, the system ingests raw RF telemetry to forecast channel utilization. If interference is predicted within the next 5-10 milliseconds, the engine pre-emptively shifts spatial signatures.

## 2. Multi-AP Coordination (ap_coordination)

The core of Wi-Fi 8. This layer allows multiple discrete Access Points to form a unified cluster, delivering Joint Transmissions (JT) to a single client at the exact same microsecond, functionally eliminating dead zones.

## 3. The Hardware Abstraction Bridge (phy_mac)

Handles the ultra-fast switching between standard 6GHz bands and experimental Sub-THz (120GHz+) frequencies. Line-of-sight blockages trigger nanosecond fallbacks to ensure zero dropped frames.
