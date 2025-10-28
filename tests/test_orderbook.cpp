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