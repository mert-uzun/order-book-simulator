#include <unordered_map>
#include <list>
#include <queue>
#include "../include/Order.h"
#include "../include/Trade.h"
#include "../include/OrderBook.h"

OrderBook::OrderBook() : buys(), sells(), order_lookup() {}

int64_t OrderBook::add_limit_order(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp) {
    Order newOrder(isBuy, priceTick, quantity, timestamp);
    int64_t newOrderId = newOrder.id;

    auto& side = isBuy ? buys : sells;

    auto& level = side[priceTick];
    auto iter = level.insert(level.end(), newOrder); // insert() returns an iterator directly to the newOrder inserted

    order_lookup[newOrderId] = std::make_tuple(priceTick, iter);

    return newOrderId;
}

void OrderBook::addMarketOrder(bool isBuy, int quantity, int64_t timestamp) {

}

void cancelOrder(int64_t orderId) {
    auto& iter_lookup = order_lookup.at(orderId);
    auto& [price, iter_to_order] = iter_lookup->second;

    
}

void modify_order(int64_t orderId, int newQuantity, int64_t timestamp) {
    if (newQuantity <= 0) {
        cancelOrder(orderId);
    }

    // get the iterator pointing to the [price, iter_to_order] tuple
    auto& iterLookup = order_lookup.find(orderId);
    if (iterLookup == order_lookup.end()) {
        return; // Meaning it didn't exist
    }

    auto& [price, iterator_to_order] = iterLookup->second;
    Order& order_to_modify = *iterator_to_order;

    if (newQuantity >= order_to_modify.quantity) {
        auto& side = order_to_modify.isBuy ? buys : sells;

        try {
            auto& price_level = side.at(order_to_modify);
        }
        catch (...) {
            std::cout << "Error occured finding the price level or order to modify."
            return;
        }

        add_limit_order(order_to_modify.isBuy, order_to_modify.priceTick, newQuantity, timestamp);

        price_level.erase(iterator_to_order); // this is the iterator pointing to the order itself 
        order_lookup.erase(iterLookup); // this is the iterator to the tuple [price, iter] stored in the unordered map
    }
    else {
        order_to_modify.quantity = newQuantity;
        order_to_modify.tsLastUpdateUs = timestamp;
    }
}

void partialFill() {}

int64_t getBestBid() {}

int64_t getBestAsk() {}

void snaphot() {}