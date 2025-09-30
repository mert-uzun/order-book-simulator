#pragma once

#include <string>
#include <cstdint>



struct Order {
        static const double tick_size;
        int64_t id;
        bool isBuy;
        bool isActive;
        int64_t priceTick;
        int quantity;
        int64_t tsCreatedUs;
        int64_t tsLastUpdateUs;

        Order(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp);
        Order(bool isBuy, int quantity, int64_t timestamp);
};