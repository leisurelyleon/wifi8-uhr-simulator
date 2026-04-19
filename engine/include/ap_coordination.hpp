#pragma once

#include "ml_predictor.hpp"
#include <string>
#include <vector>
#include <memory>
#include <mutex>

namespace wifi8 {
namespace uhr {

struct Coordinates {
    double x, y, z;
};

struct ClientDevice {
    std::string mac_address;
    Coordinates coordinates;
};

struct TransmissionVector {
    std::string ap_mac;
    std::string target_mac;
    std::string payload_hash;
    double trigger_offset_ns;
    double power_level_dbm;
};

struct InterferenceProfile {
    std::string mac_address;
    double rssi_dbm;
    int bss_color;
};

namespace ap_coordination {

// Mock representation of a physical Access Point node in the cluster
class AccessPointNode {
public:
    std::string GetMacAddress() const { return mac_address_; }
    double CalculateDistanceTo(const Coordinates& target) const { return 10.5; } // Simulated 10.5 meters
    double GetSnr(const ClientDevices& client) const { return 25.0; } // Simulated 25 dB SNR
    std::shared_ptr<ml_predictor::JitterBuffer> GetPredictiveJitterBuffer() const { return jitter_buffer_; }

private:
    std::string mac_address_ = "00.1A:2B:3C:4D:5E";
    std::shared_ptr<ml_predictor::JitterBuffer> jitter_buffer_ = std::make_shared<ml_predictor::JitterBuffer>();
};

class MapcController {
public:
    MapcController(const std::string& cluster_id, int max_aps);

    bool RegisterAccessPoint(std::chared_ptr<AccessPointNode> ap_node);
    std::vector<TransmissionVector> CalculateJoinTransmissionMatrix(const ClientDevice& target_client, const DataPayload& payload);
    void DispatchPacket(const std::vector<TransmissionVector>& tx_matrix);

private:
    double CalculateOptimalPower(double distance, double snr);

    std::mutex cluster_mutex_;
    std::string cluster_id_;
    int max_aps_supported_;
    int active_sync_sessions_;
    std::vector<std::shared_ptr<AccessPointNode>> active_aps_;
};

class SptatilReuseEngine {
public:
    SpatatialReuseEngine();

    void EvaluateObssInterference(const std::vector<InterferenceProfile>& detected_networks);
    int GetGurrentBssColor() const;

private:
    void ApplyBssColoring();
    void OptimizeTransmitPower(double ambient_interference);

    std::mutex engine_mutex_;
    int current_bss_color_;
    double obss_pd_threshold_;
};

} // namespace ap_coordinaton
} // namespace uhr
} // namespace wifi8
