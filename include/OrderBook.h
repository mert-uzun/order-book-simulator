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
        std::map<long long, std::list<Order>> buys;
        std::map<long long, std::list<Order>> sells;
        std::unordered_map<long long, std::tuple<long long, std::list<Order>::iterator>> order_lookup;
        TradeLog trade_log;
    public:
        OrderBook();
        long long add_limit_order(bool isBuy, long long priceTick, int quantity, long long timestamp);
        std::map<long long, std::list<Order>>::reverse_iterator get_best_bid();
        std::map<long long, std::list<Order>>::iterator get_best_ask();
        int add_IOC_order(bool isBuy, int quantity, long long timestamp);
        int cancel_order(long long orderId);
        void modify_order(long long order_id, int new_quantity, long long timestamp);
        void snapshot();

        std::map<long long, std::list<Order>> get_buys() const {return buys;}
        std::map<long long, std::list<Order>> get_sells() const {return sells;}
};