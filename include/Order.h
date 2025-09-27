#pragma once

#include <string>

const double tick_size = 0.01;

struct Order {
        int64_t id;
        bool isBuy;
        bool isActive;
        int64_t price_tick;
        int quantity;
        int64_t ts_created_us;
        int64_t ts_last_update_us;

        Order(int64_t id, bool isBuy, int64_t price_tick, int quantity, int64_t timestamp);
};