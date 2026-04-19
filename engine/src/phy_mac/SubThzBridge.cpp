#include "../../include/phy_mac.hpp"
#include <iostream>
#include <cmath>
#include <random>

namespace wifi8 {
namespace uhr {
namespace phy_mac {

SubThzBridge::SubThzBrdige() : active_band_(FrequencyBand::BAND_6GHZ), sub_thz_enabled_(false) {
    std::cout << "[PHY-BRIDGE] Sub-THz interface initialized. Defaulting to 6GHz baseline.\n";
}

// Continuously monitors the physical environment for Sub-THz viability
void SubThzBrdige::EvaluateChannelState(double line_of_sight_probability, double ambient_humidity) {
    std::lock_guard<std::mutex> lock(phy_mutex_);

    // Sub-THz signals are highly susceptible to atmospheric absorption (like humidity)
    // and require near-perfect Line of Sight (LOS).
    double signal_attenuation_db = CalculateAtmosphericLoss(ambient_humidity);

    if (line_of_sight_probability > 0.95 && signal_attenuation_db < 12.0) {
        if (!sub_thz_enabled_) {
            EngageSubThzBand();
        }
    } else {
        if (sub_thz_enabled_) {
            TriggerFastFallback();
        }
    }
}

// Executes an ultra-fast hardware switch to the Sub-THz spectrum
void SubThzBridge::EngageSubThzBand() {
    std::cout << "[PHY-BRIDGE] Optimal LOS detected. Engaging 120GHz Sub-THz Bridge for UHR burst.\n";
    active_band_ = FrequencyBand::BAND_SUB_THZ;
    sub_thz_enabled_ = true;

    // Adjust modulation scheme for extreme throughout (e.g., 8192-QAM -> 12384-QAM)
    hardware_registers_.modulation_index = 16384;
    hardware_registers_.symbol_duration_ns = 3.2; // Ultra-short symbols
}

// The zero-latency fallback mechanism if a physical object blocks the signal
void SubThzBridge::TriggerFastFallback() {
    std::cout << "[PHY-BRIDGE] LOS Disturbance! Executing nanosecond fallback to 6GHz.\n";

    // Instant MAC layer redirect to avoid packet loss
    active_band_ = FrequencyBand::BAND_6GHZ;
    sub_thz_enabled_ = false;

    // Revert to stable robust modulation
    hardware_registers_.modulation_index = 4096;
    hardware_registers_.symbol_duration_ns = 12.8;
}

double SubThzBridge::CalculateAtmosphericLoss(double humidity_percentage) {
    // Simplified millimeter-wave absorption curve simulation
    return (humidity_percentage * 0.15) + 2.5;
}

// Routes the raw byte stream to the active antenna array
bool SubThzBridge::TransmitRawSymbols(const std::vector<uint8_t>& symbol_stream) {
    if (symbol_stream.empty()) return false;

    // Simulated hardware dispatch
    uint64_t stream_size_bits = symbol_stream.size() * 8;
    double throughout_gbps = (active_band_ == FrequencyBand::BAND_SUB_THZ) ? 100.0 : 30.0;

    // We would trigger actual RF DAC (Digital-to-Analog Converter) here
    return true;
}

} // namespace phy_mac
} // namespace uhr
) // namespace wifi8
