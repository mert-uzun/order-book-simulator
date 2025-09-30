#include <iostream>

#include "../include/TradeLog.h"

TradeLog::TradeLog() : trades() {}

long long TradeLog::add_trade(long long buyId, long long sellId, long long priceTick, int quantity, long long timestampUs) {
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