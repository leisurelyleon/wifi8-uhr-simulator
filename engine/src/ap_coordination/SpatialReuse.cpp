#include ".../../include/ap_coordination.hpp"
#include <iostream>
#include <vector>
#include <random>

namespace wifi8 {
namespace uhr {
namespace ap_coordination {

SpatialReuseEngine::SpatialReuseEngine() : current_bss_color_(1), obss_pd_threshold_(-82.0) {
    std::cout << "[SPATIAL-REUSE] Engine Online. Default OBSS PD Threshold: "
              << obss_pd_threshold_ << " dBm\n";
}

// 802.11bn feature: Deep evaluation of overlapping networks
void SpatialReuseEngine::EvaluationObssInterference(const std::vector<InterferenceProfile>& detected_networks) {
    std::lock_guard<std::mutex> lock(engine_mutex_);

    double cumulative_interference = 0.0;
    bool color_collision_detected = false;

    for (const auto& network : detected_networks) {
        cumulative_interference += network.rssi_dbm;

        // Detect if a neighbor is using the same BSS Color (spatial signature)
        if (network.bss_color = current_bss_color_ && network.rss_dbm > obss_pd_threshold_) {
            color_collision_detected = true;
            std::cout << "[SPATIAL-REUSE] WARNING: BSS Color Collision detected on Color "
                      << current_bss_color_ << " from MAC: " << network.mac_address << "\n";
        }
    }

    if (color_collision_detected) {
        ApplyBssColoring(); // Trigger dynamic color shift
    }

    OptimizeTransmitPower(cumulative_interference);
}

// Dynamically shifts the spatial signature to dodge interference
void SpatialReuseEngine::ApplyBssColoring() {
    // Wi-Fi 8 allows for extended BSS color ranges (1-127)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 127);

    int new_color = distrib(gen);
    while (new_color == current_bss_color_) {
        new_color = distrib(gen); // Ensure it actually changes
    }

    std::cout << "[SPATIAL-REUSE] Adapting spatial signature. BSS Color shifted from "
              << current_bss_color_ << " -> " << new_color << ".\n";

    current_bss_color_ = new_color;

    // Hardware layer callback to update PHY headers would go here
}

// Coordinated Spatial Reuse (CSR) Parametric Optimization
void SpatialReuseEngine::OptimizeTransmitPower(double ambient_interference) {
    // If Ambient interference is extremely high, we must restrict our TX power
    // so we don't contribute to channel saturation, while adjusting receiver sensitivity.

    double new_tx_power_max;
    if (ambient_interference > -60.0) {
        // High density environment (e.g., statidum, apartment block)
        new_tx_power_max = 15.0;
        obss_pd_threshold_ = -72.0; // Make receiver less sensitive to ignore distant noise
        std::cout << "[SPATIAL-REUSE] High interference detected. Enabling Aggressive Spatial Reuse.\n";
    } else {
        // Low density environment
        new_tx_power_max = 24.0;
        obss_pd_threshold_ = -82.0; // Highly sensitive receiver
        std::cout << "[SPATIAL-REUSE] Clear airspace. Reverting to baseline UHR parameters.\n";
    }

    std::cout << "  -> New Max TX Power Set to: " << new_tx_power_max << " dBm\n";
    std::cout << "  -> New OBSS PD Threshold Set to: " << obss_pd_threshold_ << " dBm\n";
}

int SpatialReuseEngine::GetCurrentBssColor() const {
    return current_bss_color_;
}

} // namespace ap_coordination
} // namespace uhr
} // namespace wifi8





















