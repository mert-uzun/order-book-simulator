#pragma once

#include "Order.h"

struct Trade {
    long long tradeId;
    long long priceTick;
    int quantity;
    long long buyOrderId;
    long long sellOrderId;
    long long timestampUs;

    Trade();
    Trade(long long buyId, long long sellId, long long priceTick, int quantity, long long timestampUs);
};