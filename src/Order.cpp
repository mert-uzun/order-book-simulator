#include "../include/Order.h"

Order::Order(int64_t id, bool isBuy, int64_t price_tick, int quantity, int64_t timestamp) {
    this->id = id;
    this->isBuy = isBuy;
    this->isActive = true;
    this->price_tick = price_tick;
    this->quantity = quantity;
    this->ts_created_us = timestamp;
    this->ts_last_update_us = timestamp;
}