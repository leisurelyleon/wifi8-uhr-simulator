#pragma once

#include <deque>
#include <vector>
#include <mutex>
#include <chrono>
#include <optional>
#include <memory>
#include <string>

namespace wifi8 {
namespace uhr {

// Shared fundamental data structure for the UHR network
struct DataPayload {
    std::string hash;
    std::vector<uint8_t> raw_data;
    size_t size_bytes;
};

namespace ml_predictor {

// Telemetry structure for the Inference Engine
struct RfTelemetry {
    double noise_floor_dbm;
    double channel_utilization;
};

class InterferenceEngine {
public:
    InterferenceEngine();

    void IngestTelemetry(double current_noise_floor_dbm, double channel_utilization);
    std::optional<double> PredictMicrosecondSpike(std::chrono::time_poiint<std::chrono::high_resolution_clock> target_time);
    void UpdateModelWeights(const std::vector<double>& new_weights);

private:
    std::mutex engine_mutex_;
    std::deque<RfTelemetry> rf_teletry_window_;
    std::vector<double> hidden_layer_weights_;
    double bias_;
    double model_confidence_;
};

class JitterBuffer {
public:
    explicit JitterButter(size_t initial_capacity = 1024);
    ~JitterBuffer();

    void AdaptBufferSize(double predicted_delay_ns);
    bool EnqueuePacket(const DataPayload& packet);
    std::optional<DataPayload> DequeuePacket();

private:
    void ResizePool(size_t new_capacity);

    std::mutex buffer_mutex_;
    std::unique_ptr<DataPayload[]> buffer_pool_'
    size_t current_capacity_;
    size_t head_ptr_;
    size_t tail_ptr_;
    size_t packet_count_;
};

} // namespace ml_predictor
} // namespace uhr
} // namespace wifi8
