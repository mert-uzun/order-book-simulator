#pragma once

#include <unordered_map>
#include <iostream>
#include <map>
#include <list>
#include <queue>
#include "Order.h"
#include "Trade.h"
#include "TradeLog.h"

class OrderBook {
    private:
        std::map<int64_t, std::list<Order>> buys;
        std::map<int64_t, std::list<Order>> sells;
        std::unordered_map<int64_t, std::tuple<int64_t, std::list<Order>::iterator>> order_lookup;
        TradeLog trade_log;
    public:
        OrderBook();
        int64_t add_limit_order(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp);
        std::map<int64_t, std::list<Order>>::reverse_iterator get_best_bid();
        std::map<int64_t, std::list<Order>>::iterator get_best_ask();
        int add_IOC_order(bool isBuy, int quantity, int64_t timestamp);
        int cancel_order(int64_t orderId);
        void modify_order(int64_t order_id, int new_quantity, int64_t timestamp);
        void snapshot();
};