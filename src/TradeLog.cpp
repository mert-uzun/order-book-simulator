#include <iostream>

#include "../include/TradeLog.h"

TradeLog::TradeLog() : trades() {}

int64_t TradeLog::add_trade(int64_t buyId, int64_t sellId, int64_t priceTick, int quantity, int64_t timestampUs) {
    trades.emplace_back(buyId, sellId, priceTick, quantity, timestampUs);
    return trades.back().tradeId;
}

void TradeLog::show_trades(){
    std::cout << "TradeID | BuyID | SellID | Price | Qty | Timestamp";

    for (const auto& trade : trades) { 
        std::cout << trade.tradeId << " | " << trade.buyOrderId << " | " << trade.sellOrderId << " | " 
            << trade.priceTick * Order::tick_size << "$ | " << trade.quantity << " | " << trade.timestampUs << std::endl;
    }
}