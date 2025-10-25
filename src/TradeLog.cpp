#include <iostream>

#include "../include/TradeLog.h"

TradeLog::TradeLog() : trades() {}

long long TradeLog::add_trade(long long buy_id, long long sell_id, long long price_tick, int quantity, long long timestamp_us, bool was_instant) {
    trades.emplace_back(buy_id, sell_id, price_tick, quantity, timestamp_us, was_instant);
    return trades.back().tradeId;
}

void TradeLog::show_trades(){
    std::cout << "TradeID | BuyID | SellID | Price | Qty | Timestamp";

    for (const auto& trade : trades) { 
        std::cout << trade.tradeId << " | " << trade.buyOrderId << " | " << trade.sellOrderId << " | " 
            << trade.priceTick * Order::tick_size << "$ | " << trade.quantity << " | " << trade.timestampUs << std::endl;
    }
}