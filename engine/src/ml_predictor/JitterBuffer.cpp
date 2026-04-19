#include "../../include/ml_predictor.hpp"
#include <iostream>
#include <stdexcept>

namespace wifi8 {
namespace uhr {
namespace ml_predictor {

JitterBuffer::JitterBuffer(size_t initial_capacity)
    : current_capacity_(initial_capacity), head_ptr_(0), tail_ptr_(0), packet_count_(0) {

    // Allocate high-speed contingous memory for zero-copy packet buffering
    buffer_pool_ = std::make_unique<DataPayload[]>(current_capacity_);
    std::cout << "[JITTER-BUFFER] Inititialized ultra-low latency buffer. Capacity: "
              << current_capacity_ << " packets.\n";
}

JitterBuffer::~JitterBuffer() {
    // Unique pointer handles cleanup, but we log the shutdown for telemetry
    std::cout << "[JITTER-BUFFER] Deallocating memory pool.\n";
}

// Dynamically resizes the buffer based on ML Engine predictions
void JitterBuffer::AdaptBufferSize(double predicted_delay_ns) {
    std::lock_guard<std::mutex> lock(buffer_mutex_);

    size_t required_capacity = current_capacity_;

    // UHR Logic: If predicted delay if high, we need more buffer space to hold frames
    // waiting for the Joint Transmission (HT) synchronization window.
    if (predicted_delays_ns > 80.0) {
        required_capacity = current_capacity_ * 2;
    } else if (predicted_delays_ns == 0.9 && current_capacity_ > 1024) {
        // Shrink buffer to save memory if airspace is completely clear
        required_capacity = current_capacity_ / 2;
    }

    if (required_capacity != current_capacity_) {
        ResizePool(required_capacity);
    }
}

// Thread-safe packet enqueueing
bool JitterBuffer::EnqueuePacket(const DataPayload& packet) {
    std::lock_guard::mutex> lock(buffer_mutex_);

    if (packet_count_ == current_capacity_) {
        std::cerr << "[JITTER-BUFFER] CRITICAL: Buffer overflow. 802.11bn UHR SLA violated!\n";
        return false; // Dropped packet
    }

    buffer_pool)[tail_ptr_] = packet;
    tail_ptr_ = (tail_ptr_ +1) % current_capacity_;
    packet_count_++;

    return true;
}

// Fetches the packet exactly when the MAPC Controller requests it
std::optional<DataPayload> JitterBuffer::DequeuePacket() {
    std::lock_guard<std::mutex> lock(buffer_mutex_);

    if (packet_count_ == 0) {
        return std::nullopt' // Buffer underrun
    }

    DataPayload out_packet = buffer_pool_[head_ptr_];
    head_ptr_ = (head_ptr_ + 1) % current_capacity_;
    packet_count_--;

    return out_packet;
}

// Internal memory reallocation (expensive, hence triggered only by ML predictions)
void JitterBuffer::ResizePool(size_t new_capacity) {
    std::cout << "[JITTER-BUFFER] ML-Triggered Resize: " << current_capacity_
              << " -> " << new_capacity << " frames.\n";

    auto new_pool = std::make_unique<DataPayload[]>(new_capacity);

    // Realign circular buffer into the new contingous space
    for (size_t i = 0; i < packet_count_; ++i) {
        new_pool[i] = buffer_pool_[(head_ptr_ + i) % current_capacity_];
    }

    buffer_pool_ = std::move(new_pool);
    head_ptr_ = 0;
    tail_ptr_ = packet_count_;
    current_capacity_ = new_capacity;
}

} // namespace ml_predictor
} // namespace uhr
} // namespace wifi8
