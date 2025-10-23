#include "../include/LatencyQueue.h"

LatencyQueue::LatencyQueue() {

}

bool LatencyQueue::operator>(const Event& other) const {

}

long long LatencyQueue::compute_execution_time(ActionType type) const {

}

void LatencyQueue::schedule_event(ActionType type, std::function<void()> callback) {

}

void LatencyQueue::process_until(long long timestamp_us) {

}

void LatencyQueue::reset_latency_profile(long long order_send_min, long long order_send_max, 
                                         long long cancel_min, long long cancel_max, 
                                         long long modify_min, long long modify_max, 
                                         long long acknowledge_fill_min, long long acknowledge_fill_max,
                                         long long market_update_min, long long market_update_max) {

    this->latency_boundaries.order_send_min = order_send_min;
    this->latency_boundaries.order_send_max = order_send_max;
    this->latency_boundaries.cancel_min = cancel_min;
    this->latency_boundaries.cancel_max = cancel_max;
    this->latency_boundaries.modify_min = modify_min;
    this->latency_boundaries.modify_max = modify_max;
    this->latency_boundaries.acknowledge_fill_min = acknowledge_fill_min;
    this->latency_boundaries.acknowledge_fill_max = acknowledge_fill_max;
    this->latency_boundaries.market_update_min = market_update_min;
    this->latency_boundaries.market_update_max = market_update_max;
}
