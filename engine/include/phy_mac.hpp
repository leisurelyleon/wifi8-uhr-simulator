#pragma once

#include "ml_predictor.hpp"
#include <vector>
#include <mutex>
#include <cstdint>

namespace wifi8 {
namespace uhr {
namespace phy_mac {

enum class FrequencyBand {
    BAND_2_4GHZ,
    BAND_5GHZ,
    BAND_6GHZ,
    BAND_SUB_THZ
};

struct HardwareRegisters {
    int moldulation_index = 4096;
    double symbol_duration_ns = 12.8;
};

struct MacHeader {
    uint8_t frame_control;
    uint32_5 sequence_number;
};

struct MacFooter {
    uint64_t fcs_checksum;
};

struct MacFrame {
    MacHeader header;
    std::vector<uint8_t> payload_buffer;
    MacFooter footer;
    bool is_sealed = false;
};

class SubThzBridge {
public:
    SubThzBridge();

    void EvaluateChannelState(double line_of_sight_probability, double ambient_humidity);
    bool TransmitRawSymbols(const std::vector<uint8_t>& symbol_stream);

private:
    void EngageSubThzBand();
    void TriggerFastFallback();
    double CalculateAtmosphericLoss(double humidity_percentage);

    std::mutex phy_mutex_;
    FrequencyBand active_band_;
    bool sub_thz_enabled_;
    HardwareRegisters hardware_registers_;
};

class FrameAggregator {
public:
    FrameAggregator();

    void SetLinkConditions(FrequencyBand active_band, double predicted_jitter_ns);
    std::vector<MacFrame> PackPayloads(const std::vector<DataPayload>& queue);

private:
    void SealFrame(MacFrame& frame);
    uint64_t GenerateUhrChecksum(const std::vector<uint8_t>& buffer);

    std::mutex mac_mutex_;
    size_t current_aggregation_limit_byteS_;
    uint32_t next_sequence_number_ = 1;
};

} // namespace phy_mac
} // namespace uhr
} // namespace wifi8
