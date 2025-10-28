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
    OrderBook orderbook;

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
    OrderBook orderbook;

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
        << "Order ids does not match. Added: " << order.id << ", intended: " << order_id;
    EXPECT_EQ(order.isActive, true)
        << "Order is not active. Added: " << order.isActive << ", intended: true";
    EXPECT_EQ(order.isBuy, true)
        << "Order is not a buy order. Added: " << order.isBuy << ", intended: true";
    EXPECT_EQ(order.priceTick, 1000000)
        << "Price tick does not match. Added: " << order.priceTick << ", intended: 1000000";
    EXPECT_EQ(order.quantity, 10)
        << "Quantity does not match. Added: " << order.quantity << ", intended: 10";
    EXPECT_EQ(order.tsCreatedUs, 1)
        << "Created timestamp does not match. Added: " << order.tsCreatedUs << ", intended: 1";
    EXPECT_EQ(order.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Added: " << order.tsLastUpdateUs << ", intended: 1";
}

/**
    ============================================================
    TEST 3: AddSingleSellOrder
    ============================================================
    PURPOSE: Adds a single sell order to current order book, and check if it arrives to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddSingleSellOrder) {
    OrderBook orderbook;

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
        << "Order ids does not match. Added: " << order.id << ", intended: " << order_id;
    EXPECT_EQ(order.isActive, true)
        << "Order is not active. Added: " << order.isActive << ", intended: true";
    EXPECT_EQ(order.isBuy, false)
        << "Order is not a sell order. Added: " << order.isBuy << ", intended: false";
    EXPECT_EQ(order.priceTick, 1000000)
        << "Price tick does not match. Added: " << order.priceTick << ", intended: 1000000";
    EXPECT_EQ(order.quantity, 10)
        << "Quantity does not match. Added: " << order.quantity << ", intended: 10";
    EXPECT_EQ(order.tsCreatedUs, 1)
        << "Created timestamp does not match. Added: " << order.tsCreatedUs << ", intended: 1";
    EXPECT_EQ(order.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Added: " << order.tsLastUpdateUs << ", intended: 1";
}

/**
    ============================================================
    TEST 4: AddMultipleBuyOrders
    ============================================================
    PURPOSE: Adds multiple buy orders to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleBuyOrders) {
    OrderBook orderbook;
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
        << "Order ids does not match. Added: " << order1.id << ", intended: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Added: " << order1.isActive << ", intended: true";
    EXPECT_EQ(order1.isBuy, true)
        << "Order is not a buy order. Added: " << order1.isBuy << ", intended: true";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Added: " << order1.priceTick << ", intended: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Added: " << order1.quantity << ", intended: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Added: " << order1.tsCreatedUs << ", intended: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Added: " << order1.tsLastUpdateUs << ", intended: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);

    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Added: " << order2.id << ", intended: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Added: " << order2.isActive << ", intended: true";
    EXPECT_EQ(order2.isBuy, true)
        << "Order is not a buy order. Added: " << order2.isBuy << ", intended: true";
    EXPECT_EQ(order2.priceTick, 999999)
        << "Price tick does not match. Added: " << order2.priceTick << ", intended: 999999";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Added: " << order2.quantity << ", intended: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Added: " << order2.tsCreatedUs << ", intended: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Added: " << order2.tsLastUpdateUs << ", intended: 2";
}

/**
    ============================================================
    TEST 5: AddMultipleSellOrders
    ============================================================
    PURPOSE: Adds multiple sell orders to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleSellOrders) {
    OrderBook orderbook;

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
        << "Order ids does not match. Added: " << order1.id << ", intended: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Added: " << order1.isActive << ", intended: true";
    EXPECT_EQ(order1.isBuy, false)
        << "Order is not a sell order. Added: " << order1.isBuy << ", intended: false";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Added: " << order1.priceTick << ", intended: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Added: " << order1.quantity << ", intended: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Added: " << order1.tsCreatedUs << ", intended: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Added: " << order1.tsLastUpdateUs << ", intended: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);

    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Added: " << order2.id << ", intended: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Added: " << order2.isActive << ", intended: true";
    EXPECT_EQ(order2.isBuy, false)
        << "Order is not a sell order. Added: " << order2.isBuy << ", intended: false";
    EXPECT_EQ(order2.priceTick, 999999)
        << "Price tick does not match. Added: " << order2.priceTick << ", intended: 999999";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Added: " << order2.quantity << ", intended: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Added: " << order2.tsCreatedUs << ", intended: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Added: " << order2.tsLastUpdateUs << ", intended: 2";
}

/**
    ============================================================
    TEST 6: AddMultipleBuyOrdersWithSamePrice
    ============================================================
    PURPOSE: Adds multiple buy orders with the same price to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleBuyOrdersWithSamePrice) {
    OrderBook orderbook;

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
        << "Best bid price does not match. Added: " << orderbook.get_best_bid()->first << ", intended: 1000000";
    // Ensure only one price level is present in the orderbook
    EXPECT_EQ(orderbook.get_buys().size(), 1)
        << "Buy orders size does not match. Added: " << orderbook.get_buys().size() << ", intended: 1";
    // Ensure two orders are at the same price level
    EXPECT_EQ(orderbook.get_buys().find(1000000)->second.size(), 2)
        << "Buy orders at price 1000000 size does not match. Added: " << orderbook.get_buys().find(1000000)->second.size() << ", intended: 2";

    auto order_it1 = orderbook.get_order_lookup().find(order_id1)->second;
    Order order1 = *std::get<1>(order_it1);
    EXPECT_EQ(order1.id, order_id1)
        << "Order ids does not match. Added: " << order1.id << ", intended: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Added: " << order1.isActive << ", intended: true";
    EXPECT_EQ(order1.isBuy, true)
        << "Order is not a buy order. Added: " << order1.isBuy << ", intended: true";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Added: " << order1.priceTick << ", intended: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Added: " << order1.quantity << ", intended: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Added: " << order1.tsCreatedUs << ", intended: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Added: " << order1.tsLastUpdateUs << ", intended: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);
    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Added: " << order2.id << ", intended: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Added: " << order2.isActive << ", intended: true";
    EXPECT_EQ(order2.isBuy, true)
        << "Order is not a buy order. Added: " << order2.isBuy << ", intended: true";
    EXPECT_EQ(order2.priceTick, 1000000)
        << "Price tick does not match. Added: " << order2.priceTick << ", intended: 1000000";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Added: " << order2.quantity << ", intended: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Added: " << order2.tsCreatedUs << ", intended: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Added: " << order2.tsLastUpdateUs << ", intended: 2";
}

/**
    ============================================================
    TEST 7: AddMultipleSellOrdersWithSamePrice
    ============================================================
    PURPOSE: Adds multiple sell orders with the same price to current order book, and check if they arrive to the market with correct data
    ============================================================
 */
TEST(OrderBookTest, AddMultipleSellOrdersWithSamePrice) {
    OrderBook orderbook;

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
        << "Best ask price does not match. Added: " << orderbook.get_best_ask()->first << ", intended: 1000000";
    // Ensure only one price level is present in the orderbook
    EXPECT_EQ(orderbook.get_sells().size(), 1)
        << "Sell orders size does not match. Added: " << orderbook.get_sells().size() << ", intended: 1";
    // Ensure two orders are at the same price level
    EXPECT_EQ(orderbook.get_sells().find(1000000)->second.size(), 2)
        << "Sell orders at price 1000000 size does not match. Added: " << orderbook.get_sells().find(1000000)->second.size() << ", intended: 2";

    auto order_it1 = orderbook.get_order_lookup().find(order_id1)->second;
    Order order1 = *std::get<1>(order_it1);
    EXPECT_EQ(order1.id, order_id1)
        << "Order ids does not match. Added: " << order1.id << ", intended: " << order_id1;
    EXPECT_EQ(order1.isActive, true)
        << "Order is not active. Added: " << order1.isActive << ", intended: true";
    EXPECT_EQ(order1.isBuy, false)
        << "Order is not a sell order. Added: " << order1.isBuy << ", intended: false";
    EXPECT_EQ(order1.priceTick, 1000000)
        << "Price tick does not match. Added: " << order1.priceTick << ", intended: 1000000";
    EXPECT_EQ(order1.quantity, 10)
        << "Quantity does not match. Added: " << order1.quantity << ", intended: 10";
    EXPECT_EQ(order1.tsCreatedUs, 1)
        << "Created timestamp does not match. Added: " << order1.tsCreatedUs << ", intended: 1";
    EXPECT_EQ(order1.tsLastUpdateUs, 1)
        << "Last update timestamp does not match. Added: " << order1.tsLastUpdateUs << ", intended: 1";

    auto order_it2 = orderbook.get_order_lookup().find(order_id2)->second;
    Order order2 = *std::get<1>(order_it2);
    EXPECT_EQ(order2.id, order_id2)
        << "Order ids does not match. Added: " << order2.id << ", intended: " << order_id2;
    EXPECT_EQ(order2.isActive, true)
        << "Order is not active. Added: " << order2.isActive << ", intended: true";
    EXPECT_EQ(order2.isBuy, false)
        << "Order is not a sell order. Added: " << order2.isBuy << ", intended: false";
    EXPECT_EQ(order2.priceTick, 1000000)
        << "Price tick does not match. Added: " << order2.priceTick << ", intended: 1000000";
    EXPECT_EQ(order2.quantity, 12)
        << "Quantity does not match. Added: " << order2.quantity << ", intended: 12";
    EXPECT_EQ(order2.tsCreatedUs, 2)
        << "Created timestamp does not match. Added: " << order2.tsCreatedUs << ", intended: 2";
    EXPECT_EQ(order2.tsLastUpdateUs, 2)
        << "Last update timestamp does not match. Added: " << order2.tsLastUpdateUs << ", intended: 2";
}

/**
    ============================================================
    TEST 8: BestBidCalculation
    ============================================================
    PURPOSE: Adds multiple buy orders to current order book, and check if the best bid price is correct
    ============================================================
 */
TEST(OrderBookTest, BestBidCalculation) {
    OrderBook orderbook;
    
    long long order_id11 = orderbook.add_limit_order(true, 1000000, 11, 11);
    long long order_id12 = orderbook.add_limit_order(true, 1000000, 12, 12);

    long long order_id21 = orderbook.add_limit_order(true, 999999, 11, 21);
    long long order_id22 = orderbook.add_limit_order(true, 999999, 12, 22);

    long long order_id31 = orderbook.add_limit_order(true, 999998, 11, 31);
    long long order_id32 = orderbook.add_limit_order(true, 999998, 12, 32);

    // WE KNOW ORDERS ARE SUCCESSFULLY ADDED TO THE ORDERBOOK FROM THE PREVIOUS TESTS, NOW WE NEED TO CHECK THE BEST BID PRICE
    EXPECT_EQ(orderbook.get_best_bid()->first, 1000000)
        << "Best bid price does not match. Added: " << orderbook.get_best_bid()->first << ", prices entered: 1000000, 999999, 999998";

    // Ensure all three prices are present in the orderbook
    EXPECT_EQ(orderbook.get_buys().size(), 3)
        << "Buy orders size does not match. Added: " << orderbook.get_buys().size() << ", intended: 3";
    EXPECT_EQ(orderbook.get_buys().find(1000000)->second.size(), 2)
        << "Buy orders at price 1000000 size does not match. Added: " << orderbook.get_buys().find(1000000)->second.size() << ", intended: 2";
    EXPECT_EQ(orderbook.get_buys().find(999999)->second.size(), 2)
        << "Buy orders at price 999999 size does not match. Added: " << orderbook.get_buys().find(999999)->second.size() << ", intended: 2";
    EXPECT_EQ(orderbook.get_buys().find(999998)->second.size(), 2)
        << "Buy orders at price 999998 size does not match. Added: " << orderbook.get_buys().find(999998)->second.size() << ", intended: 2";
}

/**
    ============================================================
    TEST 9: BestAskCalculation
    ============================================================
    PURPOSE: Adds multiple sell orders to current order book, and check if the best ask price is correct
    ============================================================
 */
 TEST(OrderBookTest, BestAskCalculation) {
    OrderBook orderbook;

    long long order_id11 = orderbook.add_limit_order(false, 1000000, 11, 11);
    long long order_id12 = orderbook.add_limit_order(false, 1000000, 12, 12);

    long long order_id21 = orderbook.add_limit_order(false, 999999, 11, 21);
    long long order_id22 = orderbook.add_limit_order(false, 999999, 12, 22);

    long long order_id31 = orderbook.add_limit_order(false, 999998, 11, 31);
    long long order_id32 = orderbook.add_limit_order(false, 999998, 12, 32);

    // WE KNOW ORDERS ARE SUCCESSFULLY ADDED TO THE ORDERBOOK FROM THE PREVIOUS TESTS, NOW WE NEED TO CHECK THE BEST ASK PRICE
    EXPECT_EQ(orderbook.get_best_ask()->first, 999998)
        << "Best ask price does not match. Added: " << orderbook.get_best_ask()->first << ", prices entered: 1000000, 999999, 999998";

    // Ensure all three prices are present in the orderbook
    EXPECT_EQ(orderbook.get_sells().size(), 3)
        << "Sell orders size does not match. Added: " << orderbook.get_sells().size() << ", intended: 3";
    EXPECT_EQ(orderbook.get_sells().find(999998)->second.size(), 2)
        << "Sell orders at price 999998 size does not match. Added: " << orderbook.get_sells().find(999998)->second.size() << ", intended: 2";
    EXPECT_EQ(orderbook.get_sells().find(999999)->second.size(), 2)
        << "Sell orders at price 999999 size does not match. Added: " << orderbook.get_sells().find(999999)->second.size() << ", intended: 2";
    EXPECT_EQ(orderbook.get_sells().find(1000000)->second.size(), 2)
        << "Sell orders at price 1000000 size does not match. Added: " << orderbook.get_sells().find(1000000)->second.size() << ", intended: 2";
}
/*
    TESTS TO ADD
    
    - BestBidCalculation
    - BestAskCalculation
    - OrderMatchingBasic
    - PartialFillScenario
    - MultipleOrdersAtSamePrice
    - CancelOrderBasic
    - CancelNonExistentOrder
    
*/