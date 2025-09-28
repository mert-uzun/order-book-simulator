#pragma once

#include <unordered_map>
#include <list>
#include <queue>
#include "Order.h"
#include "Trade.h"

class OrderBook {
    private:
        std::unordered_map<int64_t, std::list<Order>> buys;
        std::unordered_map<int64_t, std::list<Order>> sells;
        std::unordered_map<int64_t, std::tuple<int64_t, std::list<Order>::iterator> order_lookup;
        
    public:
        OrderBook();
        void addLimitOrder();
        void addMarketOrder();
        void calcelOrder();
        void modifyOrder();
        void partialFill();
        int64_t getBestBid();
        int64_t getBestAsk();
        void snapshot();
}