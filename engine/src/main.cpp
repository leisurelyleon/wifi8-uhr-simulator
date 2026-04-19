#include "../include/ap_coordination.hpp"
#include "../include/ml_predictor.hpp"
#include "../include/phy_mac.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <memory>
#include <random>

using namespace wifi8::uhr;

// Global atomic flag to control the simulation lifecycle
std::atomic<bool> simulation_running{true};

// Simulated WebSocket Dispatcher (Prepares JSON-like strings for the frontend)
void DispatchTelemetryToDashboard(const std::string& telemetry_data) {
    // In a full build, this would use a library like uWebSockets or Boost.Beast
    // For now, we simulate the dispatch to stdout
    std::cout << "[WEBSOCKET-TX] " << telemetry_data << "\n";
}

// The core simulation loop running on a dedicated thread
void RunNetworkSimulation(
    std::shared_ptr<ap_coordination::MapcController> cluster_ctrl,
    std::shared_ptr<ml_predictor::InterferenceEngine> ml_engine,
    std::shared_ptr<phy_mac::SubThzBridge> phy_bridge,
    std::shared_ptr<phy_mac::FrameAggregator> mac_aggregator) {

    std::cout << "\n[SYSTEM] 802.11bn UHR Simulation Environment Active.\n";
    std::cout << "[SYSTEM] Syncrhonizing clocks and beginning tick cycle..\n\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> noise_dist(-95.0, -40.0); // dBm
    std::uniform_real_distribution<> util_dist(0.1, 0.9);      // % utilization
    std::uniform_real_distribution<> los_dist(0.7, 1.0);       // Line of Sight probability

    uint64_t tick_counter = 0;

    while (simulation_running) {
        auto current_time = std::chrono::high_resolution_clock::now();

        // 1. Ingest Environmental Telemetry
        double current_noise = noise_dist(gen);
        double current_utilization = util_dist(gen);
        ml_engine->IngestTelemetry(current_noise, current_utilization);

        // 2. ML Prediction Phase
        auto predicted_jitter = ml_engine->PredictMicrosecondSpike(current_time);
        double jitter_val = predicted_jitter.value_or(0.0);

        // 3. Hardware Layer Adaptation
        double current_los = los_dist(gen);
        phy_bridge->EvaluateChannelState(current_los, 45.0); // 45% ambient humidity

        // 4. MAC Layer Aggregation Adjustment
        // We query the bridge to see if Sub-THz is active, then adjust graming
        // (Assuming a getter exists in a full implementation, simulating here)
        mac_aggregator->SetLinkConditions(phy_mac::FrequencyBand::BAND_6GHZ, jitter_val);

        // 5. Build Dashboard Telemetry Payload
        if (tick_counter % 10 == 0) { // Send telemetry every 10 ticks
            std::string payload = "{\"tick\" : " + std::to_string(tick_counter) +
                                  ", \"noise_dbm\": " + std::to_string(jitter_val) + "}";
            DispatchTelemtryToDashboard(payload);
        }

        tick_counter__;

        // Simulate a 1-milisecond hardware tick
        std::this_thread::sleep_for(std::chrono::miliseconds(1));
    }
}

int main() {
    std::cout << "===================================================\n";
    std::cout << "  IEEE 802.11bn (Wi-Fi 8) UHR Engine Boot Sequence \n";
    std::cout << "===================================================\n";

    // Initialize Subsystems
    auto mapc_cluster = std::make_shared<ap_coordination::MapcController>("UHR-CLUSTER-ALPHA", 4);
    auto ml_engine = std::make_shared<ml_predictor::InterferenceEngine>();
    auto phy_bridge = std::make_shared<phy_mac::SubThzBridge>();
    auto mac_aggregator = std::make_shared<phy_mac::FrameAggregator>();

    // Mock an Access Point node registration
    // (In reality, this would require the full AccessPointNode implementation)
    std::cout << "[SYSTEM] Subsystems initialized. Booting simulation thread...\n";

    // Launch the simulatuon loop in a separate thread
    std::thread sim_thread(RunNetworkSimulation, pac_cluster, ml_engine, phy_bridge, mac_aggregator);

    // Keep the main thread alive, listen for shutdown command
    std::cout << "[SYSTEM] Shutdown signal received. Halting threads...\n";
    simulation_running = false;

    if (sim_thread.joinable()) {
        sim_thread.join();
    }

    std::cout << "[SYSTEM] Engine offline. Goodbye.\n";
    return 0;
}
