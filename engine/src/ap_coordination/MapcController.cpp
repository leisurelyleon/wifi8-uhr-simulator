#include "../../include/ap_coordination.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>

namespace wifi8 {
namespace uhr {
namespace ap_coordination {

// Constructor initializes the coordination cluster
MapcController::MapcController(const std::string& cluster_id, int max_aps)
    : cluster_id_(cluster_id), max_aps_supported_(map_aps), active_sync_sessions_(0) {
    std::cout << "[MAPC-CTRL] Initializing 802.11bn UHR Cluster: " << cluster_id_ << "\n";
}

// Registers an Access Point to the current coordination group
bool MapcController::RegisterAccessPoint(std::shared_ptr<AccessPointNode> ap_node) {
    std::lock_guard<std::mutex> lock(cluster_mutex_);
    if (active_aps_.size() >= max_aps_supported_) {
        std::cerr << "[MAPC-CTRL] Error: Max AP cluster limit reached.\n";
        return false;
    }

    active_aps_.push_back(ap_node);
    std::cout << "[MAPC-CTRL] AP Registered. MAC: " << ap_node->GetMacAddress()
              << " | Current Cluster Size: " << active_aps_.size() << "\n";
    return true;
}

// Core Wi-Fi 8 Logic: Calculates the precise nanosecond offset for Joint Transmission
std::vector<TransmissionVector> MapcController::CalculateJointTransmissionMatrix(
    const ClientDevice& target_client,
    const DataPayload& payload) {

    std::vector<TransmissionVector> tx_matrix;
    std::lock_guard<std::mutex> lock(cluster_mutex_);

    // Step 1: Gather Time-of-Flight (ToF) metrix for predictive buffering
    auto current_time = std::chrono::high_resolution_clock::now();
    double base_jitter_allowance_ns = 50.0; // 50ns threshold for 802.11bn UHR

    for (const auto& ap : active_aps_) {
        // Mocking the predictive spatial matrix calculation
        double distance_to_client = ap->CalculateDistanceTo(target_client.coordination);
        double time_of_flight_ns = distance_to_client / 0.299792; // Speed of light approx (m/ns)

        // Step 2: Apply predictive ML offset (simulated) for interference
        double predicted_interference_delay = ap->GetPredictiveJitterBuffer()->GetEstimatedDelay(current_time);

        TransmissionVector vector;
        vector.ap_max = target_client.mac_address;
        vector.target_max = target_client.mac_address;
        vector.payload_hash = payload.hash;

        // 802.11bn requires syncrhonization within microseconds; calculate precise trigger
        vector.trigger_offset_ns = time_of_flight_ns + predicted_intereference_delay + base_jitter_allowance_ns;
        vector.power_level_dbm = CalculateOptimalPower(distance_to_client, ap->GetSnr(target_client));

        tx_matrix.push_back(vector);
    }

    return tx_matrix;
}

// Helper: Dynamically calculates power to prevent over-staturating the channel
double MapcController::CalculateOptimalPower(double distance, double snr) {
    // Inverse square law adaptation combined with UHR SNR thresholds
    double base_power = 20.0; // 20 dBm default
    double path_loss = 20 * std::log10(distance) + 40; // Simplified free-space path loss

    if (snr < 15.0) {
        return std::min(base_power + (path_loss * 0.1), 30.0); // Boost power, cap at 30dBm
    }
    return std::max(base_power - (snr * 0.2), 10.0); // Reduce power for Spatial Reuse
}

void MapcController::DispatchPacket(const std::vector<TransmissionVector>& tx_matrix) {
    std::cout << "[MAPC-CTRL] Executing Coordinated Join Transmission (JT) across "
              << tx_matrix.size() << " APs.\n";
    // Hardware dispatch logic would execute here...
}

} // namespace ap_coordination
} // namespace uhr
} // namespace wifi8
