#pragma once

#include <functional>
#include <queue>
#include <random>
#include <vector>

class LatencyQueue {
    private:
        struct Event {
            long long time_to_execute;
            
            std::function<void(long long time_to_execute)> callback;

            bool operator>(const Event& other) const;

            Event(long long time_to_execute, std::function<void(long long time_to_execute)> callback) : time_to_execute(time_to_execute), callback(callback) {}
        };

            struct LatencyBoundaries {
            long long order_send_min, order_send_max;
            long long cancel_min, cancel_max;
            long long modify_min, modify_max;
            long long acknowledge_fill_min, acknowledge_fill_max;
            long long market_update_min, market_update_max;
        };

        std::random_device rd;
        std::mt19937 engine;

        std::priority_queue<Event, std::vector<Event>, std::greater<Event>> event_queue;
        LatencyBoundaries latency_boundaries;

    public:
        enum ActionType {
            ORDER_SEND,
            CANCEL,
            MODIFY,
            ACKNOWLEDGE_FILL,
            MARKET_UPDATE
        };

        LatencyQueue();

        long long compute_execution_latency(ActionType type);

        template<typename F>
        void schedule_event(long long timestamp_us, ActionType type, F&& func) {
            long long execution_latency = compute_execution_latency(type);
            event_queue.emplace(timestamp_us + execution_latency, func);
        }

        void process_until(long long timestamp_us);
        void reset_latency_profile(long long order_send_min, long long order_send_max, 
                                   long long cancel_min, long long cancel_max, 
                                   long long modify_min, long long modify_max, 
                                   long long acknowledge_fill_min, long long acknowledge_fill_max,
                                   long long market_update_min, long long market_update_max);

        // Getters
        const auto& get_event_queue() const {
            return event_queue;
        }

        const auto& get_latency_boundaries() const {
            return latency_boundaries;
        }

        bool is_empty() const {
            return event_queue.empty();
        }

        long long get_order_send_min() const { return latency_boundaries.order_send_min; }
        long long get_order_send_max() const { return latency_boundaries.order_send_max; }
        long long get_cancel_min() const { return latency_boundaries.cancel_min; }
        long long get_cancel_max() const { return latency_boundaries.cancel_max; }
        long long get_modify_min() const { return latency_boundaries.modify_min; }
        long long get_modify_max() const { return latency_boundaries.modify_max; }
        long long get_acknowledge_fill_min() const { return latency_boundaries.acknowledge_fill_min; }
        long long get_acknowledge_fill_max() const { return latency_boundaries.acknowledge_fill_max; }
        long long get_market_update_min() const { return latency_boundaries.market_update_min; }
        long long get_market_update_max() const { return latency_boundaries.market_update_max; }
};