#pragma once

#include <unordered_map>
#include <list>
#include <queue>
#include "Order.h"
#include "Trade.h"

class OrderBook {
    private:
        std::map<int64_t, std::list<Order>> buys;
        std::map<int64_t, std::list<Order>> sells;
        std::unordered_map<int64_t, std::tuple<int64_t, std::list<Order>::iterator> order_lookup;
        
    public:
        OrderBook();
        void add_limit_order();
        void addMarketOrder();
        void cancel_order();
        void modify_order();
        void partialFill();
        int64_t getBestBid();
        int64_t getBestAsk();
        void snapshot();
};