#include <algorithm>
#include <cmath>

#include "../include/Order.h"
#include "../include/Trade.h"
#include "../include/OrderBook.h"

OrderBook::OrderBook() : buys(), sells(), order_lookup(), trade_log() {}

int OrderBook::match_order(int64_t orderId) {
    auto iter_lookup = order_lookup.find(orderId);
    auto& [price, iter_to_order] = iter_lookup->second;

    Order& order_to_match = *iter_to_order;

    auto& side = order_to_match.isBuy ? buys : sells;
    auto& match_side = order_to_match.isBuy ? sells : buys;


    return 1;
}

int64_t OrderBook::add_limit_order(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp) {
    Order new_order(isBuy, priceTick, quantity, timestamp);

    if (isBuy) {
        if (priceTick >= get_best_ask()->first) {
            auto& orders_at_price = get_best_ask()->second;
            Order& matched_order = orders_at_price.front();

            int matched_quantity = std::min(new_order.quantity, matched_order.quantity);
            new_order.quantity -= matched_quantity;
            matched_order.quantity -= matched_quantity;

            if (new_order.quantity == 0) {
                Trade exec_trade(new_order.id, matched_order.id, priceTick, matched_quantity, timestamp);

                return 0; // As a sign that new order entered the system and find its match immediately
            }
        }
    }
    else {
        if (priceTick <= get_best_bid()->first) {
            auto& orders_at_price = get_best_bid()->second;
        }
    }
    /*
    Check if given price has a match in the current market (if its a buy >= best_ask | if its a sell <= best_bid) 
        if yes: reduce the min price from the new order and the matched one, if new order goes zero->end, if matched goes zero->continue exhausting the iterator.
        if no: put the object directly in the data holders
    
    
    */
    
    int64_t newOrderId = new_order.id;

    auto& side = isBuy ? buys : sells;

    auto& level = side[priceTick];
    auto iter = level.insert(level.end(), new_order); // insert() returns an iterator directly to the newOrder inserted

    order_lookup[newOrderId] = std::make_tuple(priceTick, iter);

    return newOrderId;
}

void OrderBook::addMarketOrder(bool isBuy, int quantity, int64_t timestamp) {

}

int OrderBook::cancel_order(int64_t orderId) {
    auto iter_lookup = order_lookup.find(orderId); // .find(key) returns a temp iterator to the corresponding <key, value> std::pair
    if (iter_lookup == order_lookup.end()){
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

void OrderBook::modify_order(int64_t order_id, int new_quantity, int64_t timestamp) {
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
            std::cout << "Error occured finding the price level or order to modify.";
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

void OrderBook::partialFill() {}

/**
 * @brief Gets the best bid (highest buy price in the market along with the list of orders in that price)
 * @return returns an iterator pointing to the highest bid in the market as a <price, list<Order>> pair
 */
 std::map<int64_t, std::list<Order>>::reverse_iterator OrderBook::get_best_bid() {
    auto bestBuy = buys.rbegin(); 
    return bestBuy;
}

/**
 * @brief Gets the best ask (lowest sell price in the market along with the list of orders in that price)
 * @return returns an iterator pointing to the lowest ask in the market as a <price, list<Order>> pair
 */
 std::map<int64_t, std::list<Order>>::iterator OrderBook::get_best_ask() {
    auto bestSell = sells.begin();
    return bestSell;
}

void OrderBook::snapshot() {
    std::cout << "**************************" << std::endl;

    std::cout << "Buys:" << std::endl;
    for (auto iter_buy_map = buys.rbegin(); iter_buy_map != buys.rend(); ++iter_buy_map) {
        double price_shown = ((iter_buy_map->first) * Order::tick_size);
        std::cout << "Orders at price " << price_shown << "$ : [";
        auto& buy_list_at_price = iter_buy_map->second;
        for (auto iter_buy_list = (buy_list_at_price).begin(); iter_buy_list != buy_list_at_price.end(); ++iter_buy_list) {
            Order& order_at_iter = *iter_buy_list;
            std::cout << "id=" << order_at_iter.id << ":" << order_at_iter.quantity;

            if (&order_at_iter != &(buy_list_at_price.back())) {
                std::cout << ", ";
            }
        }

        std::cout << "]" << std::endl;
    }

    std::cout << "**************************" << std::endl;

    std::cout << "Sells:"<< std::endl;
    for (auto iter_sell_map = sells.begin(); iter_sell_map != sells.end(); ++iter_sell_map) {
        double price_shown = ((iter_sell_map->first) * Order::tick_size);
        std::cout << "Orders at price " << price_shown << "$ : [";
        auto& sell_list_at_price = iter_sell_map->second;
        for (auto iter_sell_list = (sell_list_at_price).begin(); iter_sell_list != sell_list_at_price.end(); ++iter_sell_list) {
            Order& order_at_iter = *iter_sell_list;
            std::cout << "id=" << order_at_iter.id << ":" << order_at_iter.quantity;

            if (&order_at_iter != &sell_list_at_price.back()) {
                std::cout << ", ";
            }
        }

        std::cout << "]" << std::endl;
    }

    std::cout << "**************************";
}