#pragma once

#include <string>
#include <cstdint>

const double tick_size = 0.01;

struct Order {
        int64_t id;
        bool isBuy;
        bool isActive;
        int64_t priceTick;
        int quantity;
        int64_t tsCreatedUs;
        int64_t tsLastUpdateUs;

        Order(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp);
};