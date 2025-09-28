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

int cancel_order(int64_t orderId) {
    auto iter_lookup = order_lookup.find(orderId); // .find(key) returns a temp iterator to the corresponding <key, value> std::pair
    if (iter_lookup == order;_lookup.end()){
        std::cout << "This order doesn't exist.";
        return 1;
    }

    auto& [price, iter_to_order] = iter_lookup->second;

    Order& order_to_delete = *iter_to_order;

    auto& side = order_to_delete.isBuy ? buys : sells;

    auto price_level = side.find(order_to_delete.priceTick);
    if (price_level == side.end()){
        std::cout << "Error occured finding the price level or order to modify.";
        return 1;
    }
    auto& price_level_list = price_level->second;

    price_level_list.erase(iter_to_order);
    order_lookup.erase(iter_lookup);

    return 0;
}

void modify_order(int64_t order_id, int new_quantity, int64_t timestamp) {
    if (new_quantity <= 0) {
        cancel_order(order_id);
    }

    // get the iterator pointing to the [price, iter_to_order] tuple
    auto iter_lookup = order_lookup.find(order_id);
    if (iter_lookup == order_lookup.end()) {
        return; // Meaning it didn't exist
    }

    auto& [price, iterator_to_order] = iter_lookup->second;
    Order& order_to_modify = *iterator_to_order;

    if (new_quantity >= order_to_modify.quantity) {
        auto& side = order_to_modify.isBuy ? buys : sells;

        
        auto price_level = side.find(order_to_modify.priceTick);
        if (price_level == side.end()) {
            std::cout << "Error occured finding the price level or order to modify."
            return;
        }
        auto& price_level_list = price_level->second;

        bool is_order_buy = order_to_modify.isBuy;
        int64_t order_price = order_to_modify.priceTick;

        price_level_list.erase(iterator_to_order); // this is the iterator pointing to the order itself 
        order_lookup.erase(iter_lookup); // this is the iterator to the tuple [price, iter] stored in the unordered map

        add_limit_order(is_order_buy, order_price, new_quantity, timestamp);
    }
    else {
        order_to_modify.quantity = new_quantity;
        order_to_modify.tsLastUpdateUs = timestamp;
    }
}

void partialFill() {}

int64_t getBestBid() {}

int64_t getBestAsk() {}

void snaphot() {}