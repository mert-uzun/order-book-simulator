#include <gtest/gtest.h>
#include "../include/OrderBook.h"

TEST(OrderBookTest, EmptyOrderBookTest) {
    OrderBook orderbook;

    EXPECT_TRUE(orderbook.get_buys().empty())
        << "New orderbook should have no buy orders.";

    EXPECT_TRUE(orderbook.get_sells().empty())
        << "New orderbook should have no sell orders.";
}

/* 
    TESTS TO ADD
    
    - AddSingleBuyOrder
    - AddSingleSellOrder
    - BestBidCalculation
    - BestAskCalculation
    - OrderMatchingBasic
    - PartialFillScenario
    - MultipleOrdersAtSamePrice
    - CancelOrderBasic
    - CancelNonExistentOrder
    
*/