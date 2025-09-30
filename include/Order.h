#pragma once

#include <string>
#include <cstdint>



struct Order {
        static const double tick_size;
        long long id;
        bool isBuy;
        bool isActive;
        long long priceTick;
        int quantity;
        long long tsCreatedUs;
        long long tsLastUpdateUs;

        Order(bool isBuy, long long priceTick, int quantity, long long timestamp);
        Order(bool isBuy, int quantity, long long timestamp);
};