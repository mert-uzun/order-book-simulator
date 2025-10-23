#pragma once

#include <functional>
#include <queue>
#include <vector>

class LatencyQueue {
    private:
        struct Event {
            long long time_to_execute;
            std::function<void()> callback;
        };

        enum ActionType {
            ORDER_SEND,
            CANCEL,
            MODIFY,
            ACKNOWLEDGE_FILL,
            MARKET_UPDATE
        };

        struct LatencyBoundaries {
            long long order_send_min, order_send_max;
            long long cancel_min, cancel_max;
            long long modify_min, modify_max;
            long long acknowledge_fill_min, acknowledge_fill_max;
            long long market_update_min, market_update_max;
        };

        std::priority_queue<Event, std::vector<Event>, std::greater<Event>> event_queue;
        LatencyBoundaries latency_boundaries;

    public:
        LatencyQueue();
        bool operator>(const Event& other) const;
        long long compute_execution_time(ActionType type) const;
        void schedule_event(ActionType type, std::function<void()> callback);
        void process_until(long long timestamp_us);
        void reset_latency_profile(long long, long long, 
                                   long long, long long,
                                   long long, long long,
                                   long long, long long,
                                   long long, long long);

        // GETTERS
        const auto& get_event_queue() const {
            return event_queue;
        }

        const auto& get_latency_boundaries() const {
            return latency_boundaries;
        }

        bool is_empty() const {
            return event_queue.empty();
        }
};