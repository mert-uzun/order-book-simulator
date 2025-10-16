#pragma once

#include "Order.h"

struct Trade {
    long long tradeId;
    long long priceTick;
    int quantity;
    long long buyOrderId;
    long long sellOrderId;
    long long timestampUs;
    bool was_instant;

    Trade();
    Trade(long long buyId, long long sellId, long long priceTick, int quantity, long long timestampUs, bool was_instant);
};