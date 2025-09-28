#include <unordered_map>
#include <list>
#include <queue>
#include "../include/Order.h"
#include "../include/Trade.h"
#include "../include/OrderBook.h"

OrderBook::OrderBook() : buys(), sells(), orderLookup() {}

int64_t OrderBook::addLimitOrder(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp) {
    Order newOrder(isBuy, priceTick, quantity, timestamp);
    int64_t newOrderId = newOrder.id;

    if (isBuy) {
        auto& level = buys[priceTick];
        auto it = level.insert(level.end(), newOrder); // insert() returns an iterator directly to the newOrder inserted

        orderLookup[newOrderId] = std::make_tuple(priceTick, it);
    } 
    else {
        auto& level = sells[priceTick];
        auto it = level.insert(level.end(), newOrder);

        orderLookup[newOrderId] = std::make_tuple(priceTick, it);
    }

    return newOrderId;
}

void OrderBook::addMarketOrder() {

}
