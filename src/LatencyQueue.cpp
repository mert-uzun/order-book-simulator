#include "../include/LatencyQueue.h"
#include <random>

LatencyQueue::LatencyQueue() : rd(), engine(rd), event_queue() {
    reset_latency_profile(50, 200,
                          30, 150, 
                          40, 180, 
                          100, 400, 
                          50, 150);
}

bool LatencyQueue::Event::operator>(const Event& other) const {
    return this->time_to_execute > other.time_to_execute;
}

long long LatencyQueue::compute_execution_latency(ActionType type) {
    switch (type) {
        case ORDER_SEND: {
            std::uniform_int_distribution<long long> distribution(latency_boundaries.order_send_min, latency_boundaries.order_send_max);
            long long execution_time = distribution(engine);
            return execution_time;
        }            
        case CANCEL: {
            std::uniform_int_distribution<long long> distibution(latency_boundaries.cancel_min, latency_boundaries.cancel_max);
            long long execution_time = distibution(engine);
            return execution_time;
        }
        case MODIFY: {
            std::uniform_int_distribution<long long> distribution(latency_boundaries.modify_min, latency_boundaries.modify_max);
            long long execution_time = distribution(engine);
            return execution_time;
        }
        case ACKNOWLEDGE_FILL: {
            std::uniform_int_distribution<long long> distribution(latency_boundaries.acknowledge_fill_min, latency_boundaries.acknowledge_fill_max);
            long long execution_time = distribution(engine);
            return execution_time;
        }
        case MARKET_UPDATE: {
            std::uniform_int_distribution<long long> distribution(latency_boundaries.market_update_min, latency_boundaries.market_update_max);
            long long execution_time = distribution(engine);
            return execution_time;
        }            
        default:
            return 100;
    }
}

template<typename F>
void LatencyQueue::schedule_event(long long timestamp_us, ActionType type, F&& func) {
    long long execution_latency = compute_execution_latency(type);
    event_queue.emplace(timestamp_us + execution_latency, func);
}

void LatencyQueue::process_until(long long timestamp_us) {
    while (!event_queue.empty() && timestamp_us > event_queue.top().time_to_execute) {
        auto event = event_queue.top();
        event_queue.pop();
        event.callback();
    }
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
