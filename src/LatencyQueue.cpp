#include "../include/LatencyQueue.h"
#include <random>
#include <iostream>

LatencyQueue::LatencyQueue() : rd(), engine(rd()), event_queue() {
    // Initialize with defaults
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

void LatencyQueue::process_until(long long timestamp_us) {
    while (!event_queue.empty() && timestamp_us > event_queue.top().time_to_execute) {
        long long time_to_exec = event_queue.top().time_to_execute;
        auto event = event_queue.top();
        event_queue.pop();
        event.callback(time_to_exec);
    }
}

void LatencyQueue::reset_latency_profile(long long order_send_min, long long order_send_max, 
                                         long long cancel_min, long long cancel_max, 
                                         long long modify_min, long long modify_max, 
                                         long long acknowledge_fill_min, long long acknowledge_fill_max,
                                         long long market_update_min, long long market_update_max) {

    if (order_send_min >= 0 && order_send_max >= 0 && order_send_min <= order_send_max) {
        this->latency_boundaries.order_send_min = order_send_min;
        this->latency_boundaries.order_send_max = order_send_max;    
    }
    else {
        std::cout << "Invalid latency boundaries for order send, values are not updated. Order send min: " << order_send_min << " Order send max: " << order_send_max << std::endl;
    }

    if (cancel_min >= 0 && cancel_max >= 0 && cancel_min <= cancel_max) {
        this->latency_boundaries.cancel_min = cancel_min;
        this->latency_boundaries.cancel_max = cancel_max;
    }
    else {
        std::cout << "Invalid latency boundaries for cancel, values are not updated. Cancel min: " << cancel_min << " Cancel max: " << cancel_max << std::endl;
    }

    if (modify_min >= 0 && modify_max >= 0 && modify_min <= modify_max) {
        this->latency_boundaries.modify_min = modify_min;
        this->latency_boundaries.modify_max = modify_max;
    }
    else {
        std::cout << "Invalid latency boundaries for modify, values are not updated. Modify min: " << modify_min << " Modify max: " << modify_max << std::endl;
    }

    if (acknowledge_fill_min >= 0 && acknowledge_fill_max >= 0 && acknowledge_fill_min <= acknowledge_fill_max) {
        this->latency_boundaries.acknowledge_fill_min = acknowledge_fill_min;
        this->latency_boundaries.acknowledge_fill_max = acknowledge_fill_max;
    }
    else {
        std::cout << "Invalid latency boundaries for acknowledge fill, values are not updated. Acknowledge fill min: " << acknowledge_fill_min << " Acknowledge fill max: " << acknowledge_fill_max << std::endl;
    }

    if (market_update_min >= 0 && market_update_max >= 0 && market_update_min <= market_update_max) {
        this->latency_boundaries.market_update_min = market_update_min;
        this->latency_boundaries.market_update_max = market_update_max;
    }
    else {
        std::cout << "Invalid latency boundaries for market update, values are not updated. Market update min: " << market_update_min << " Market update max: " << market_update_max << std::endl;
    }
}
