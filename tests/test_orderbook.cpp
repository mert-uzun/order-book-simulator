#include <gtest/gtest.h>
#include "../include/OrderBook.h"

/**
    ============================================================
    TEST 1: EmptyOrderBookTest
    ============================================================
    PURPOSE: Verify that a newly created OrderBook is empty
    ============================================================
 */
TEST(OrderBookTest, EmptyOrderBookTest) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    EXPECT_TRUE(orderbook.get_buys().empty())
        << "New orderbook should have no buy orders.";

    EXPECT_TRUE(orderbook.get_sells().empty())
        << "New orderbook should have no sell orders.";
}

/**
    ============================================================
    TEST 2: AddSingleBuyOrder
    ============================================================
    PURPOSE: Adds a single buy order to current order book, and check if it arrives to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddSingleBuyOrder) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    long long order_id = orderbook.add_limit_order(true, 1000000, 10, 1);

    // Be sure order is added to both data members of the orderbook
    EXPECT_NE(order_id, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_FALSE(orderbook.get_buys().empty())
        << "Buy orders are empty. Order was not added to the buys map.";

    auto order_it = orderbook.get_best_bid();
    Order order = order_it->second.back();

    EXPECT_EQ(order.id, order_id)
        << "Order ids does not match. Result: " << order.id << ", expected: " << order_id;
    EXPECT_EQ(order.isActive, true)
        << "Order is not active. Result: " << order.isActive << ", expected: true";
    EXPECT_EQ(order.isBuy, true)
        << "Order is not a buy order. Result: " << order.isBuy << ", expected: true";
    EXPECT_EQ(order.priceTick, 1000000)
        << "Price tick does not match. Result: " << order.priceTick << ", expected: 1000000";
    EXPECT_EQ(order.quantity, 10)
        << "Quantity does not match. Result: " << order.quantity << ", expected: 10";
    EXPECT_EQ(order.tsCreatedUs, 1)
        << "Created timestamp does not match. Result: " << order.tsCreatedUs << ", expected: 1";
    EXPECT_EQ(order.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Result: " << order.tsLastUpdateUs << ", expected: 1";
}

/**
    ============================================================
    TEST 3: AddSingleSellOrder
    ============================================================
    PURPOSE: Adds a single sell order to current order book, and check if it arrives to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddSingleSellOrder) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    long long order_id = orderbook.add_limit_order(false, 1000000, 10, 1);

    // Be sure order is added to both data members of the orderbook
    EXPECT_NE(order_id, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_FALSE(orderbook.get_sells().empty())
        << "Sell orders are empty. Order was not added to the sells map.";

    auto order_it = orderbook.get_best_ask();
    Order order = order_it->second.back();

    EXPECT_EQ(order.id, order_id)
        << "Order ids does not match. Result: " << order.id << ", expected: " << order_id;
    EXPECT_EQ(order.isActive, true)
        << "Order is not active. Result: " << order.isActive << ", expected: true";
    EXPECT_EQ(order.isBuy, false)
        << "Order is not a sell order. Result: " << order.isBuy << ", expected: false";
    EXPECT_EQ(order.priceTick, 1000000)
        << "Price tick does not match. Result: " << order.priceTick << ", expected: 1000000";
    EXPECT_EQ(order.quantity, 10)
        << "Quantity does not match. Result: " << order.quantity << ", expected: 10";
    EXPECT_EQ(order.tsCreatedUs, 1)
        << "Created timestamp does not match. Result: " << order.tsCreatedUs << ", expected: 1";
    EXPECT_EQ(order.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Result: " << order.tsLastUpdateUs << ", expected: 1";
}

/**
    ============================================================
    TEST 4: AddMultipleBuyOrders
    ============================================================
    PURPOSE: Adds multiple buy orders to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleBuyOrders) {
    Metrics metrics;
    OrderBook orderbook(metrics);
    long long order_id1 = orderbook.add_limit_order(true, 1000000, 10, 1);
    long long order_id2 = orderbook.add_limit_order(true, 999999, 12, 2);

    // Be sure orders are added to both data members of the orderbook
    EXPECT_NE(order_id1, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id1), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_NE(order_id2, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id2), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_FALSE(orderbook.get_buys().empty())
        << "Buy orders are empty. Orders were not added to the buys map.";

    auto order_it1 = orderbook.get_order_lookup().find(order_id1)->second;
    Order order1 = *std::get<1>(order_it1);

    EXPECT_EQ(order1.id, order_id1)
        << "Order ids does not match. Result: " << order1.id << ", expected: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Result: " << order1.isActive << ", expected: true";
    EXPECT_EQ(order1.isBuy, true)
        << "Order is not a buy order. Result: " << order1.isBuy << ", expected: true";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Result: " << order1.priceTick << ", expected: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Result: " << order1.quantity << ", expected: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Result: " << order1.tsCreatedUs << ", expected: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Result: " << order1.tsLastUpdateUs << ", expected: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);

    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Result: " << order2.id << ", expected: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Result: " << order2.isActive << ", expected: true";
    EXPECT_EQ(order2.isBuy, true)
        << "Order is not a buy order. Result: " << order2.isBuy << ", expected: true";
    EXPECT_EQ(order2.priceTick, 999999)
        << "Price tick does not match. Result: " << order2.priceTick << ", expected: 999999";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Result: " << order2.quantity << ", expected: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Result: " << order2.tsCreatedUs << ", expected: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Result: " << order2.tsLastUpdateUs << ", expected: 2";
}

/**
    ============================================================
    TEST 5: AddMultipleSellOrders
    ============================================================
    PURPOSE: Adds multiple sell orders to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleSellOrders) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    long long order_id1 = orderbook.add_limit_order(false, 1000000, 10, 1);
    long long order_id2 = orderbook.add_limit_order(false, 999999, 12, 2);

    // Be sure orders are added to both data members of the orderbook
    EXPECT_NE(order_id1, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id1), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_NE(order_id2, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id2), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_FALSE(orderbook.get_sells().empty())
        << "Sell orders are empty. Orders were not added to the sells map.";

    auto order_it1 = orderbook.get_order_lookup().find(order_id1)->second;
    Order order1 = *std::get<1>(order_it1);

    EXPECT_EQ(order1.id, order_id1)
        << "Order ids does not match. Result: " << order1.id << ", expected: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Result: " << order1.isActive << ", expected: true";
    EXPECT_EQ(order1.isBuy, false)
        << "Order is not a sell order. Result: " << order1.isBuy << ", expected: false";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Result: " << order1.priceTick << ", expected: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Result: " << order1.quantity << ", expected: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Result: " << order1.tsCreatedUs << ", expected: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Result: " << order1.tsLastUpdateUs << ", expected: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);

    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Result: " << order2.id << ", expected: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Result: " << order2.isActive << ", expected: true";
    EXPECT_EQ(order2.isBuy, false)
        << "Order is not a sell order. Result: " << order2.isBuy << ", expected: false";
    EXPECT_EQ(order2.priceTick, 999999)
        << "Price tick does not match. Result: " << order2.priceTick << ", expected: 999999";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Result: " << order2.quantity << ", expected: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Result: " << order2.tsCreatedUs << ", expected: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Result: " << order2.tsLastUpdateUs << ", expected: 2";
}

/**
    ============================================================
    TEST 6: AddMultipleBuyOrdersWithSamePrice
    ============================================================
    PURPOSE: Adds multiple buy orders with the same price to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleBuyOrdersWithSamePrice) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    long long order_id1 = orderbook.add_limit_order(true, 1000000, 10, 1);
    long long order_id2 = orderbook.add_limit_order(true, 1000000, 12, 2);

    // Be sure orders are added to both data members of the orderbook
    EXPECT_NE(order_id1, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id1), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_NE(order_id2, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id2), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_FALSE(orderbook.get_buys().empty())
        << "Buy orders are empty. Orders were not added to the buys map.";

    // Ensure best bid price is correct
    EXPECT_EQ(orderbook.get_best_bid()->first, 1000000)
        << "Best bid price does not match. Result: " << orderbook.get_best_bid()->first << ", expected: 1000000";
    // Ensure only one price level is present in the orderbook
    EXPECT_EQ(orderbook.get_buys().size(), 1)
        << "Buy orders size does not match. Result: " << orderbook.get_buys().size() << ", expected: 1";
    // Ensure two orders are at the same price level
    EXPECT_EQ(orderbook.get_buys().find(1000000)->second.size(), 2)
        << "Buy orders at price 1000000 size does not match. Result: " << orderbook.get_buys().find(1000000)->second.size() << ", expected: 2";

    auto order_it1 = orderbook.get_order_lookup().find(order_id1)->second;
    Order order1 = *std::get<1>(order_it1);
    EXPECT_EQ(order1.id, order_id1)
        << "Order ids does not match. Result: " << order1.id << ", expected: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Result: " << order1.isActive << ", expected: true";
    EXPECT_EQ(order1.isBuy, true)
        << "Order is not a buy order. Result: " << order1.isBuy << ", expected: true";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Result: " << order1.priceTick << ", expected: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Result: " << order1.quantity << ", expected: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Result: " << order1.tsCreatedUs << ", expected: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Result: " << order1.tsLastUpdateUs << ", expected: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);
    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Result: " << order2.id << ", expected: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Result: " << order2.isActive << ", expected: true";
    EXPECT_EQ(order2.isBuy, true)
        << "Order is not a buy order. Result: " << order2.isBuy << ", expected: true";
    EXPECT_EQ(order2.priceTick, 1000000)
        << "Price tick does not match. Result: " << order2.priceTick << ", expected: 1000000";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Result: " << order2.quantity << ", expected: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Result: " << order2.tsCreatedUs << ", expected: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Result: " << order2.tsLastUpdateUs << ", expected: 2";
}

/**
    ============================================================
    TEST 7: AddMultipleSellOrdersWithSamePrice
    ============================================================
    PURPOSE: Adds multiple sell orders with the same price to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleSellOrdersWithSamePrice) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    long long order_id1 = orderbook.add_limit_order(false, 1000000, 10, 1);
    long long order_id2 = orderbook.add_limit_order(false, 1000000, 12, 2);

    // Be sure orders are added to both data members of the orderbook
    EXPECT_NE(order_id1, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id1), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_NE(order_id2, -1)
        << "Order id is -1. Valid id is not generated, order was not added to the market.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id2), orderbook.get_order_lookup().end())
        << "Order id is not found in the order lookup. Order was not added to the order_lookup map.";
    EXPECT_FALSE(orderbook.get_sells().empty())
        << "Sell orders are empty. Orders were not added to the sells map.";

    // Ensure best ask price is correct
    EXPECT_EQ(orderbook.get_best_ask()->first, 1000000)
        << "Best ask price does not match. Result: " << orderbook.get_best_ask()->first << ", expected: 1000000";
    // Ensure only one price level is present in the orderbook
    EXPECT_EQ(orderbook.get_sells().size(), 1)
        << "Sell orders size does not match. Result: " << orderbook.get_sells().size() << ", expected: 1";
    // Ensure two orders are at the same price level
    EXPECT_EQ(orderbook.get_sells().find(1000000)->second.size(), 2)
        << "Sell orders at price 1000000 size does not match. Result: " << orderbook.get_sells().find(1000000)->second.size() << ", expected: 2";

    auto order_it1 = orderbook.get_order_lookup().find(order_id1)->second;
    Order order1 = *std::get<1>(order_it1);
    EXPECT_EQ(order1.id, order_id1)
        << "Order ids does not match. Result: " << order1.id << ", expected: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Result: " << order1.isActive << ", expected: true";
    EXPECT_EQ(order1.isBuy, false)
        << "Order is not a sell order. Result: " << order1.isBuy << ", expected: false";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Result: " << order1.priceTick << ", expected: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Result: " << order1.quantity << ", expected: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Result: " << order1.tsCreatedUs << ", expected: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Result: " << order1.tsLastUpdateUs << ", expected: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);
    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Result: " << order2.id << ", expected: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Result: " << order2.isActive << ", expected: true";
    EXPECT_EQ(order2.isBuy, false)
        << "Order is not a sell order. Result: " << order2.isBuy << ", expected: false";
    EXPECT_EQ(order2.priceTick, 1000000)
        << "Price tick does not match. Result: " << order2.priceTick << ", expected: 1000000";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Result: " << order2.quantity << ", expected: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Result: " << order2.tsCreatedUs << ", expected: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Result: " << order2.tsLastUpdateUs << ", expected: 2";
}

/**
    ============================================================
    TEST 8: BestBidCalculation
    ============================================================
    PURPOSE: Adds multiple buy orders to current order book, and check if the best bid price is correct
    ============================================================
 */
TEST(OrderBookTest, BestBidCalculation) {
    Metrics metrics;
    OrderBook orderbook(metrics);
    
    long long order_id11 = orderbook.add_limit_order(true, 1000000, 11, 11);
    long long order_id12 = orderbook.add_limit_order(true, 1000000, 12, 12);

    long long order_id21 = orderbook.add_limit_order(true, 999999, 11, 21);
    long long order_id22 = orderbook.add_limit_order(true, 999999, 12, 22);

    long long order_id31 = orderbook.add_limit_order(true, 999998, 11, 31);
    long long order_id32 = orderbook.add_limit_order(true, 999998, 12, 32);

    // WE KNOW ORDERS ARE SUCCESSFULLY ADDED TO THE ORDERBOOK FROM THE PREVIOUS TESTS, NOW WE NEED TO CHECK THE BEST BID PRICE
    EXPECT_EQ(orderbook.get_best_bid()->first, 1000000)
        << "Best bid price does not match. Result: " << orderbook.get_best_bid()->first << ", prices entered: 1000000, 999999, 999998";

    // Ensure all three prices are present in the orderbook
    EXPECT_EQ(orderbook.get_buys().size(), 3)
        << "Buy orders size does not match. Result: " << orderbook.get_buys().size() << ", expected: 3";
    EXPECT_EQ(orderbook.get_buys().find(1000000)->second.size(), 2)
        << "Buy orders at price 1000000 size does not match. Result: " << orderbook.get_buys().find(1000000)->second.size() << ", expected: 2";
    EXPECT_EQ(orderbook.get_buys().find(999999)->second.size(), 2)
        << "Buy orders at price 999999 size does not match. Result: " << orderbook.get_buys().find(999999)->second.size() << ", expected: 2";
    EXPECT_EQ(orderbook.get_buys().find(999998)->second.size(), 2)
        << "Buy orders at price 999998 size does not match. Result: " << orderbook.get_buys().find(999998)->second.size() << ", expected: 2";
}

/**
    ============================================================
    TEST 9: BestAskCalculation
    ============================================================
    PURPOSE: Adds multiple sell orders to current order book, and check if the best ask price is correct
    ============================================================
 */
 TEST(OrderBookTest, BestAskCalculation) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    long long order_id11 = orderbook.add_limit_order(false, 1000000, 11, 11);
    long long order_id12 = orderbook.add_limit_order(false, 1000000, 12, 12);

    long long order_id21 = orderbook.add_limit_order(false, 999999, 11, 21);
    long long order_id22 = orderbook.add_limit_order(false, 999999, 12, 22);

    long long order_id31 = orderbook.add_limit_order(false, 999998, 11, 31);
    long long order_id32 = orderbook.add_limit_order(false, 999998, 12, 32);

    // WE KNOW ORDERS ARE SUCCESSFULLY ADDED TO THE ORDERBOOK FROM THE PREVIOUS TESTS, NOW WE NEED TO CHECK THE BEST ASK PRICE
    EXPECT_EQ(orderbook.get_best_ask()->first, 999998)
        << "Best ask price does not match. Result: " << orderbook.get_best_ask()->first << ", prices entered: 1000000, 999999, 999998";

    // Ensure all three prices are present in the orderbook
    EXPECT_EQ(orderbook.get_sells().size(), 3)
        << "Sell orders size does not match. Result: " << orderbook.get_sells().size() << ", expected: 3";
    EXPECT_EQ(orderbook.get_sells().find(999998)->second.size(), 2)
        << "Sell orders at price 999998 size does not match. Result: " << orderbook.get_sells().find(999998)->second.size() << ", expected: 2";
    EXPECT_EQ(orderbook.get_sells().find(999999)->second.size(), 2)
        << "Sell orders at price 999999 size does not match. Result: " << orderbook.get_sells().find(999999)->second.size() << ", expected: 2";
    EXPECT_EQ(orderbook.get_sells().find(1000000)->second.size(), 2)
        << "Sell orders at price 1000000 size does not match. Result: " << orderbook.get_sells().find(1000000)->second.size() << ", expected: 2";
}

/**
    ============================================================
    TEST 10: OrderMatchingWithLimitBuyAndLimitSell
    ============================================================
    PURPOSE: Adds a buy order and a sell order with matching price levels to current order book, and check if they match and the order book and trade log is updated correctly. Checks it with limit orders on both sides.
    ============================================================
 */
TEST(OrderBookTest, OrderMatchingWithLimitBuyAndLimitSell) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    // ============================================================
    // FIRST MATCHING SCENARIO - First bid then ask
    // ============================================================

    // First, add a limit buy order and a limit sell order with matching price levels and check if they match and the order book is updated correctly
    long long order_id11 = orderbook.add_limit_order(true, 1000000, 10, 11);
    long long order_id12 = orderbook.add_limit_order(true, 999999, 10, 12);
    long long order_id21 = orderbook.add_limit_order(false, 999999, 10, 21);

    // Check if the sell and buy maps are updated correctly
    EXPECT_EQ(orderbook.get_buys().find(1000000), orderbook.get_buys().end())
        << "Buy orders at price level 1000000 are not erased after getting emptied.";
    EXPECT_EQ(orderbook.get_sells().find(999999), orderbook.get_sells().end())
        << "Sell orders at price level 999999 are not erased after getting emptied.";
    EXPECT_NE(orderbook.get_buys().find(999999), orderbook.get_buys().end())
        << "Buy orders at price level 999999 are erased without matching.";

    // Check if the order lookup is updated correctly
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id11), orderbook.get_order_lookup().end())
        << "Order 11 is still in the order lookup after matching.";
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id21), orderbook.get_order_lookup().end())
        << "Order 21 is still in the order lookup after matching.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id12), orderbook.get_order_lookup().end())
        << "Order 12 is not found in the order lookup, but it shouldn't have been erased since it didn't match.";
    
    // Check if the trade log is updated correctly
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 1)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 1";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, order_id11)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << order_id11;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, order_id21)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << order_id21;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 1000000)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 1000000";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 10)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 10";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 21)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 21";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, false)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: false";

    // Adding a new sell order to trigger order12
    long long order_id22 = orderbook.add_limit_order(false, 999998, 10, 22);

    // Check if sell and buy maps are updated correctly after the second match
    EXPECT_EQ(orderbook.get_buys().find(999999), orderbook.get_buys().end())
        << "Buy orders at price 999999 are not erased after getting emptied.";
    EXPECT_EQ(orderbook.get_sells().find(999998), orderbook.get_sells().end())
        << "Sell orders at price 999998 are not erased after getting emptied.";

    // Check if the order lookup is updated correctly after the second match
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id12), orderbook.get_order_lookup().end())
        << "Order 12 is still in the order lookup after matching.";
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id22), orderbook.get_order_lookup().end())
        << "Order 22 is still in the order lookup after matching.";

    // Check if the trade log is updated correctly after the second match
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 2)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 2";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, order_id12)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << order_id12;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, order_id22)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << order_id22;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 999999)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 999999";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 10)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 10";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 22)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 22";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, false)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: false";

    // ============================================================
    // SECOND MATCHING SCENARIO - First ask then bid
    // ============================================================

    // Second, add a limit buy order and an IOC sell order with matching price levels and check if they match and the order book is updated correctly
    long long order_id31 = orderbook.add_limit_order(false, 1000000, 10, 31);
    long long order_id32 = orderbook.add_limit_order(false, 1000001, 10, 32);
    long long order_id41 = orderbook.add_limit_order(true, 1000001, 10, 41);

    // Check if the sell and buy maps are updated correctly
    EXPECT_EQ(orderbook.get_buys().find(1000001), orderbook.get_buys().end())
        << "Buy orders at price 1000001 are not erased after getting emptied.";
    EXPECT_EQ(orderbook.get_sells().find(1000000), orderbook.get_sells().end())
        << "Sell orders at price 1000000 are not erased after getting emptied.";
    EXPECT_NE(orderbook.get_sells().find(1000001), orderbook.get_sells().end())
        << "Sell orders at price 1000001 are erased without matching.";

    // Check if the order lookup is updated correctly
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id31), orderbook.get_order_lookup().end())
        << "Order 31 is still in the order lookup after matching.";
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id41), orderbook.get_order_lookup().end())
        << "Order 41 is still in the order lookup after matching.";
    EXPECT_NE(orderbook.get_order_lookup().find(order_id32), orderbook.get_order_lookup().end())
        << "Order 32 is not found in the order lookup, but it shouldn't have been erased since it didn't match.";
    
    // Check if the trade log is updated correctly
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 3)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 3";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, order_id41)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << order_id41;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, order_id31)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << order_id31;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 1000000)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 1000000";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 10)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 10";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 41)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 41";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, false)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: false";

    // Adding a new buy order to trigger order32
    long long order_id42 = orderbook.add_limit_order(true, 1000002, 10, 42);

    // Check if the sell and buy maps are updated correctly after the second match
    EXPECT_EQ(orderbook.get_sells().find(1000001), orderbook.get_sells().end())
        << "Sell orders at price 1000001 are not erased after getting emptied.";
    EXPECT_EQ(orderbook.get_buys().find(1000002), orderbook.get_buys().end())
        << "Buy orders at price 1000002 are not erased after getting emptied.";
    
    // Check if the order lookup is updated correctly after the second match
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id32), orderbook.get_order_lookup().end())
        << "Order 32 is still in the order lookup after matching.";
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id42), orderbook.get_order_lookup().end())
        << "Order 42 is still in the order lookup after matching.";
    
    // Check if the trade log is updated correctly after the second match
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 4)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 4";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, order_id42)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << order_id42;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, order_id32)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << order_id32;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 1000001)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 1000001";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 10)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 10";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 42)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 42";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, false)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: false";
}

/**
    ============================================================
    TEST 11: OrderMatchingWithIOCOrders
    ============================================================
    PURPOSE: Adds limit buy orders and an IOC sell order with matching price levels and check if they match and the order book and trade log is updated correctly. Checks it with limit orders on both sides.
    ============================================================
 */
TEST(OrderBookTest, OrderMatchingWithIOCOrders) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    // ============================================================
    // FIRST SCENARIO - One limit buy then one IOC sell
    // ============================================================

    // First add one limit buy order and fill with one IOC sell order
    long long limit_buy_order_id1 = orderbook.add_limit_order(true, 1000000, 10, 1);
    long long ioc_sell_order_id1 = orderbook.add_IOC_order(false, 10, 2);

    // Check orderbook.buys, order lookup, and trade log that these are updated correctly
    EXPECT_EQ(orderbook.get_buys().find(1000000), orderbook.get_buys().end())
        << "Resting buy order is still in the orderbook.buys after supposedly being filled by an IOC sell order.";
    EXPECT_EQ(orderbook.get_order_lookup().find(limit_buy_order_id1), orderbook.get_order_lookup().end())
        << "Resting buy order is still in the order lookup after supposedly being filled by an IOC sell order.";
    EXPECT_TRUE(orderbook.get_sells().empty())
        << "Sell orders are not empty after a limit buy order and an IOC sell order.";

    // Check if the trade log is updated correctly
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 1)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 1";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, limit_buy_order_id1)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << limit_buy_order_id1;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, ioc_sell_order_id1)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << ioc_sell_order_id1;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 1000000)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 1000000";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 10)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 10";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 2)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 2";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, true)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: true";

        
    // ============================================================
    // SECOND SCENARIO - Two limit buys then one IOC sell, which fills the first limit buy fully and the second partially
    // ============================================================

    // First add two limit buy orders and fill with one IOC sell order
    long long limit_buy_order_id2 = orderbook.add_limit_order(true, 1000002, 10, 3);
    long long limit_buy_order_id3 = orderbook.add_limit_order(true, 1000001, 10, 4);
    long long ioc_sell_order_id2 = orderbook.add_IOC_order(false, 15, 5);
    
    // Check orderbook.buys, order lookup, and trade log that these are updated correctly
    EXPECT_EQ(orderbook.get_buys().find(1000002), orderbook.get_buys().end())
        << "Resting buy order is still in the orderbook.buys after supposedly being filled by an IOC sell order.";
    EXPECT_EQ(orderbook.get_order_lookup().find(limit_buy_order_id2), orderbook.get_order_lookup().end())
        << "Resting buy order is still in the order lookup after supposedly being filled by an IOC sell order.";
    EXPECT_NE(orderbook.get_buys().find(1000001), orderbook.get_buys().end())
        << "Resting buy order is not found in the orderbook.buys after being only partiallyfilled by an IOC sell order.";
    EXPECT_NE(orderbook.get_order_lookup().find(limit_buy_order_id3), orderbook.get_order_lookup().end())
        << "Resting buy order is still in the order lookup after being only partially filled by an IOC sell order.";
    EXPECT_TRUE(orderbook.get_sells().empty())
        << "Sell orders are not empty after two limit buy orders and an IOC sell order.";

    // Check trade log size and access to second and third trades in the trade log, ones the system just executed above
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 3);

    auto& trades = orderbook.get_trade_log().get_trades();
    auto iter_to_second_trade = std::next(trades.rbegin());
    Trade& trade2 = *iter_to_second_trade;

    auto iter_to_third_trade = trades.rbegin();
    Trade& trade3 = *iter_to_third_trade;

    // Check if the second and third trades are created correctly
    EXPECT_EQ(trade2.buyOrderId, limit_buy_order_id2)
        << "Second trade buy order iddoes not match. Result: " << trade2.buyOrderId << ", expected: " << limit_buy_order_id2;
    EXPECT_EQ(trade2.sellOrderId, ioc_sell_order_id2)
        << "Second trade sell order iddoes not match. Result: " << trade2.sellOrderId << ", expected: " << ioc_sell_order_id2;
    EXPECT_EQ(trade2.priceTick, 1000002)
        << "Second trade price tick does not match. Result: " << trade2.priceTick << ", expected: 1000001";
    EXPECT_EQ(trade2.quantity, 10)
        << "Second trade quantity does not match. Result: " << trade2.quantity << ", expected: 10";
    EXPECT_EQ(trade2.timestampUs, 5)
        << "Second trade timestamp does not match. Result: " << trade2.timestampUs << ", expected: 5";
    EXPECT_EQ(trade2.was_instant, true)
        << "Second trade 'was instant' flag does not match. Result: " << trade2.was_instant << ", expected: true";

    EXPECT_EQ(trade3.buyOrderId, limit_buy_order_id3)
        << "Third trade buy order iddoes not match. Result: " << trade3.buyOrderId << ", expected: " << limit_buy_order_id3;
    EXPECT_EQ(trade3.sellOrderId, ioc_sell_order_id2)
        << "Third trade sell order iddoes not match. Result: " << trade3.sellOrderId << ", expected: " << ioc_sell_order_id2;
    EXPECT_EQ(trade3.priceTick, 1000001)
        << "Third trade price tick does not match. Result: " << trade3.priceTick << ", expected: 1000001";
    EXPECT_EQ(trade3.quantity, 5)
        << "Third trade quantity does not match. Result: " << trade3.quantity << ", expected: 10";
    EXPECT_EQ(trade3.timestampUs, 5)
        << "Third trade timestamp does not match. Result: " << trade3.timestampUs << ", expected: 5";
    EXPECT_EQ(trade3.was_instant, true)
        << "Third trade 'was instant' flag does not match. Result: " << trade3.was_instant << ", expected: true";

    // Cancel the remaining limit buy order for a clean setup for third scenario
    orderbook.cancel_order(limit_buy_order_id3);

    // ============================================================
    // THIRD SCENARIO - One limit sell then one IOC buy
    // ============================================================

    // First add one limit sell order and fill with one IOC buy order
    long long limit_sell_order_id1 = orderbook.add_limit_order(false, 1000003, 10, 6);
    long long ioc_buy_order_id1 = orderbook.add_IOC_order(true, 10, 7);

    // Check orderbook.sells, order lookup, and trade log that these are updated correctly
    EXPECT_EQ(orderbook.get_sells().find(1000003), orderbook.get_sells().end())
        << "Resting sell order is still in the orderbook.sells after supposedly being filled by an IOC buy order.";
    EXPECT_EQ(orderbook.get_order_lookup().find(limit_sell_order_id1), orderbook.get_order_lookup().end())
        << "Resting sell order is still in the order lookup after supposedly being filled by an IOC buy order.";
    EXPECT_TRUE(orderbook.get_buys().empty())
        << "Buy orders are not empty after a limit sell order and an IOC buy order.";

    // Check if the trade log is updated correctly
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 4)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 4";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, ioc_buy_order_id1)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << ioc_buy_order_id1;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, limit_sell_order_id1)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << limit_sell_order_id1;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 1000003)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 1000003";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 10)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 10";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 7)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 7";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, true)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: true";

    // ============================================================
    // FOURTH SCENARIO - Two limit sells then one IOC buy, which fills the first limit sellfully and the second partially
    // ============================================================

    // First add two limit sell orders and fill with one IOC buy order
    long long limit_sell_order_id2 = orderbook.add_limit_order(false, 1000004, 10, 8);
    long long limit_sell_order_id3 = orderbook.add_limit_order(false, 1000005, 10, 9);
    long long ioc_buy_order_id2 = orderbook.add_IOC_order(true, 15, 10);

    // Check orderbook.sells, order lookup, and trade log that these are updated correctly
    EXPECT_EQ(orderbook.get_sells().find(1000004), orderbook.get_sells().end())
        << "Resting sell order is still in the orderbook.sells after supposedly being filled by an IOC buy order.";
    EXPECT_EQ(orderbook.get_order_lookup().find(limit_sell_order_id2), orderbook.get_order_lookup().end())
        << "Resting sell order is still in the order lookup after supposedly being filled by an IOC buy order.";
    EXPECT_NE(orderbook.get_sells().find(1000005), orderbook.get_sells().end())
        << "Resting sell order is not found in the orderbook.sells after being only partially filled by an IOC buy order.";
    EXPECT_NE(orderbook.get_order_lookup().find(limit_sell_order_id3), orderbook.get_order_lookup().end())
        << "Resting sell order is not found in the order lookup after being only partially filled by an IOC buy order.";
    EXPECT_TRUE(orderbook.get_buys().empty())
        << "Buy orders are not empty after two limit sell orders and an IOC buy order.";

    // Check trade log size and access to fifth and sixth trades in the trade log, ones the system just executed above
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 6);

    auto iter_to_fifth_trade = std::next(trades.rbegin());
    Trade& trade5 = *iter_to_fifth_trade;
    auto iter_to_sixth_trade = trades.rbegin();
    Trade& trade6 = *iter_to_sixth_trade;

    // Check if the fifth and sixth trades are created correctly
    EXPECT_EQ(trade5.buyOrderId, ioc_buy_order_id2)
        << "Fifth trade buy order iddoes not match. Result: " << trade5.buyOrderId << ", expected: " << ioc_buy_order_id2;
    EXPECT_EQ(trade5.sellOrderId, limit_sell_order_id2)
        << "Fifth trade sell order iddoes not match. Result: " << trade5.sellOrderId << ", expected: " << limit_sell_order_id2;
    EXPECT_EQ(trade5.priceTick, 1000004)
        << "Fifth trade price tick does not match. Result: " << trade5.priceTick << ", expected: 1000004";
    EXPECT_EQ(trade5.quantity, 10)
        << "Fifth trade quantity does not match. Result: " << trade5.quantity << ", expected: 10";
    EXPECT_EQ(trade5.timestampUs, 10)
        << "Fifth trade timestamp does not match. Result: " << trade5.timestampUs << ", expected: 10";
    EXPECT_EQ(trade5.was_instant, true)
        << "Fifth trade 'was instant' flag does not match. Result: " << trade5.was_instant << ", expected: true";

    EXPECT_EQ(trade6.buyOrderId, ioc_buy_order_id2)
        << "Sixth trade buy order iddoes not match. Result: " << trade6.buyOrderId << ", expected: " << ioc_buy_order_id2;
    EXPECT_EQ(trade6.sellOrderId, limit_sell_order_id3)
        << "Sixth trade sell order iddoes not match. Result: " << trade6.sellOrderId << ", expected: " << limit_sell_order_id3;
    EXPECT_EQ(trade6.priceTick, 1000005)
        << "Sixth trade price tick does not match. Result: " << trade6.priceTick << ", expected: 1000005";
    EXPECT_EQ(trade6.quantity, 5)
        << "Sixth trade quantity does not match. Result: " << trade6.quantity << ", expected: 10";
    EXPECT_EQ(trade6.timestampUs, 10)
        << "Sixth trade timestamp does not match. Result: " << trade6.timestampUs << ", expected: 10";
    EXPECT_EQ(trade6.was_instant, true)
        << "Sixth trade 'was instant' flag does not match. Result: " << trade6.was_instant << ", expected: true";
}

/**
    ============================================================
    TEST 12: PartialFillScenario
    ============================================================
    PURPOSE: Tests the partial fill scenario with limit buy and IOC sell orders.
    ============================================================
 */
TEST(OrderBookTest, PartialFillScenario) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    // Add a limit buy order and partially fill it with an IOC sell order
    long long limit_buy_order_id = orderbook.add_limit_order(true, 1000000, 10, 1);
    long long ioc_sell_order_id = orderbook.add_IOC_order(false, 5, 2);

    // Check if the orderbook and order lookup is updated correctly
    EXPECT_NE(orderbook.get_order_lookup().find(limit_buy_order_id), orderbook.get_order_lookup().end())
        << "Limit buy order is not found in the order lookup after partial fill, but it shouldn't be erased.";
    EXPECT_NE(orderbook.get_buys().find(1000000), orderbook.get_buys().end())
        << "Limit buy order is not found in the orderbook.buys after partial fill, but it shouldn't be erased.";

    // Check if remaining quantity is updated correctly
    EXPECT_EQ(std::get<1>(orderbook.get_order_lookup().find(limit_buy_order_id)->second)->quantity, 5)
        << "Remaining quantity does not match (limit buy / IOC sell). Result: " << std::get<1>(orderbook.get_order_lookup().find(limit_buy_order_id)->second)->quantity << ", expected: 5";

    // Check if the trade log is updated correctly
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 1)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 1";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, limit_buy_order_id)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << limit_buy_order_id;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, ioc_sell_order_id)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << ioc_sell_order_id;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 1000000)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 1000000";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 5)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 5";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 2)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 2";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, true)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: false";

    // Add a new sell limit order with a non-matching price level with the resting buy order. Then add an IOC buy order to trigger the new limit sell order.
    long long new_sell_order_id = orderbook.add_limit_order(false, 1000001, 10, 3);
    long long ioc_buy_order_id = orderbook.add_IOC_order(true, 5, 4);

    // Check if the orderbook and order lookup is updated correctly
    EXPECT_NE(orderbook.get_order_lookup().find(new_sell_order_id), orderbook.get_order_lookup().end())
        << "New sell order is not found in the order lookup after partial fill, but it shouldn't be erased.";
    EXPECT_NE(orderbook.get_sells().find(1000001), orderbook.get_sells().end())
        << "New sell order is not found in the orderbook.sells after partial fill, but it shouldn't be erased.";
    
    // Check if remaining quantity is updated correctly
    EXPECT_EQ(std::get<1>(orderbook.get_order_lookup().find(new_sell_order_id)->second)->quantity, 5)
        << "Remaining quantity does not match (limit sell / IOC buy). Result: " << std::get<1>(orderbook.get_order_lookup().find(new_sell_order_id)->second)->quantity << ", expected: 5";

    // Check if the trade log is updated correctly
    EXPECT_EQ(orderbook.get_trade_log().get_trades().size(), 2)
        << "Trade log size does not match. Result: " << orderbook.get_trade_log().get_trades().size() << ", expected: 2";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().buyOrderId, ioc_buy_order_id)
        << "Trade buy order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().buyOrderId << ", expected: " << ioc_buy_order_id;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().sellOrderId, new_sell_order_id)
        << "Trade sell order iddoes not match. Result: " << orderbook.get_trade_log().get_trades().back().sellOrderId << ", expected: " << new_sell_order_id;
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().priceTick, 1000001)
        << "Trade price tick does not match. Result: " << orderbook.get_trade_log().get_trades().back().priceTick << ", expected: 1000001";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().quantity, 5)
        << "Trade quantity does not match. Result: " << orderbook.get_trade_log().get_trades().back().quantity << ", expected: 5";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().timestampUs, 4)
        << "Trade timestamp does not match. Result: " << orderbook.get_trade_log().get_trades().back().timestampUs << ", expected: 4";
    EXPECT_EQ(orderbook.get_trade_log().get_trades().back().was_instant, true)
        << "Trade 'was instant' flag does not match. Result: " << orderbook.get_trade_log().get_trades().back().was_instant << ", expected: false";
}

/**
    ============================================================
    TEST 13: CancelOrderBasic
    ============================================================
    PURPOSE: Tests the cancel order basic scenario with limit buy and limit sell orders.
    ============================================================
 */
TEST(OrderBookTest, CancelOrderBasic) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    // Add one limit buy order and one limit sell order with non-matching price levels for them to rest in the market
    long long order_id1 = orderbook.add_limit_order(true, 1000000, 10, 1);
    long long order_id2 = orderbook.add_limit_order(false, 2000000, 10, 2);

    // WE KNOW FROM PREVIOUS TESTS THAT THESE ORDERS ARE SUCCESSFULLY ADDED TO THE MARKET

    // Cancel the buy order and check if it is successfully removed from the order book and the order lookup
    orderbook.cancel_order(order_id1);
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id1), orderbook.get_order_lookup().end())
        << "Order 1 is still in the order lookup after cancellation.";
    EXPECT_EQ(orderbook.get_buys().find(1000000), orderbook.get_buys().end())
        << "After the cancellation, buy order might still be in the orderbook.buys, or the list for the price level might not be successfully deleted after being emptied.";
    
    // Cancel the sell order and check if it is successfully removed from the order book and the order lookup
    orderbook.cancel_order(order_id2);
    EXPECT_EQ(orderbook.get_order_lookup().find(order_id2), orderbook.get_order_lookup().end())
        << "Order 2 is still in the order lookup after cancellation.";
    EXPECT_EQ(orderbook.get_sells().find(2000000), orderbook.get_sells().end())
        << "After the cancellation, sell order might still be in the orderbook.sells, or the list for the price level might not be successfully deleted after being emptied.";
}

/**
    ============================================================
    TEST 14: CancelNonExistentOrder
    ============================================================
    PURPOSE: Tests the cancel non existent order scenario with limit buy and limit sell orders.
    ============================================================
 */
TEST(OrderBookTest, CancelNonExistentOrder) {
    Metrics metrics;
    OrderBook orderbook(metrics);

    // Cancel a non existent order and check if it returns 1, indicating the order was not found. If it returns 0, this means somehow the order was successfully cancelled and there might be a bug.
    EXPECT_EQ(orderbook.cancel_order(-1), 1)
        << "Cancelling a non existent order should be realized in the cancel function and return 1, indicating the task was aborted.";
    EXPECT_NE(orderbook.cancel_order(-1), 0)
        << "Cancelling an order should return 0 only if the order was successfully found and deleted.";
}
