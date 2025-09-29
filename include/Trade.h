#pragma once

#include "Order.h"

struct Trade {
    int64_t tradeId;
    int64_t priceTick;
    int quantity;
    int64_t buyOrderId;
    int64_t sellOrderId;
    int64_t timestampUs;

    Trade();
    Trade(int64_t buyId, int64_t sellId, int64_t priceTick, int quantity, int64_t timestampUs);
};