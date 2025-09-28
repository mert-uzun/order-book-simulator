#include <unordered_map>
#include <list>
#include <queue>
#include "../include/Order.h"
#include "../include/Trade.h"
#include "../include/OrderBook.h"

OrderBook::OrderBook() {
    // Start everything with 10 initial buckets
    this->buys(10)
    this->sells(10)
    this->order_lookup(10)
}

void OrderBook::addLimitOrder(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp) {
    if (isBuy) {
        buys.emplace(priceTick, Order(isBuy, ))
    }
    
}

void OrderBook::addMarketOrder() {

}
