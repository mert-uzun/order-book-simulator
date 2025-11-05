#include <gtest/gtest.h>
#include "../include/MarketEngine.h"

/**
    ============================================================
    TEST 1: MarketEngineInitialization
    ============================================================
    PURPOSE: Verify the MarketEngine initializes correctly with an empty order book.
    ============================================================
*/
TEST(MarketEngineTest, MarketEngineInitialization) {
    MarketEngine marketengine(100, 2, 1000, 3, 500000, 1000, 2, 1.0, 0.3);
    EXPECT_EQ(marketengine.get_orderbook().get_buys().size(), 0)
        << "There should be no buy orders in the orderbook after initialization. Current size: " << marketengine.get_orderbook().get_buys().size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_sells().size(), 0)
        << "There should be no sell orders in the orderbook after initialization. Current size: " << marketengine.get_orderbook().get_sells().size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_active_buy_order_id(), -1)
        << "There should be no active buy order after initialization. Current active buy order id: " << marketengine.get_strategy().get_active_buy_order_id() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_active_sell_order_id(), -1)
        << "There should be no active sell order after initialization. Current active sell order id: " << marketengine.get_strategy().get_active_sell_order_id() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().order_cache.size(), 0)
        << "There should be no orders in the order cache after initialization. Current order cache size: " << marketengine.get_strategy().get_metrics().order_cache.size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().gross_traded_qty, 0)
        << "There should be no trades executed after initialization. Current gross traded quantity: " << marketengine.get_strategy().get_metrics().gross_traded_qty << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_trade_price_ticks, 0)
        << "There should be no last trade price after initialization. Current last trade price: " << marketengine.get_strategy().get_metrics().last_trade_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_mark_price_ticks, 1000)
        << "There should be no last mark price after initialization. Current last mark price: " << marketengine.get_strategy().get_metrics().last_mark_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_return_bucket_start_us, 0)
        << "There should be no last return bucket start time after initialization. Current last return bucket start time: " << marketengine.get_strategy().get_metrics().last_return_bucket_start_us << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_return_bucket_total_pnl_ticks, 0)
        << "There should be no last return bucket total PnL after initialization. Current last return bucket total PnL: " << marketengine.get_strategy().get_metrics().last_return_bucket_total_pnl_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().current_best_bid_price_ticks, 1000)
        << "There should be no current best bid price after initialization. Current current best bid price: " << marketengine.get_strategy().get_metrics().current_best_bid_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().current_best_ask_price_ticks, 1000)
        << "There should be no current best ask price after initialization. Current current best ask price: " << marketengine.get_strategy().get_metrics().current_best_ask_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().volatility, 1.0)
        << "There should be no volatility after initialization. Current volatility: " << marketengine.get_strategy().get_metrics().volatility << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().sharpe_ratio, 0.0)
        << "There should be no Sharpe ratio after initialization. Current Sharpe ratio: " << marketengine.get_strategy().get_metrics().sharpe_ratio << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().gross_profit, 0.0)
        << "There should be no gross profit after initialization. Current gross profit: " << marketengine.get_strategy().get_metrics().gross_profit << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().gross_loss, 0.0)
        << "There should be no gross loss after initialization. Current gross loss: " << marketengine.get_strategy().get_metrics().gross_loss << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().win_rate, 0.0)
        << "There should be no win rate after initialization. Current win rate: " << marketengine.get_strategy().get_metrics().win_rate << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().resting_attempted_qty, 0)
        << "There should be no resting attempted quantity after initialization. Current resting attempted quantity: " << marketengine.get_strategy().get_metrics().resting_attempted_qty << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().resting_cancelled_qty, 0)
        << "There should be no resting cancelled quantity after initialization. Current resting cancelled quantity: " << marketengine.get_strategy().get_metrics().resting_cancelled_qty << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().resting_filled_qty, 0)
        << "There should be no resting filled quantity after initialization. Current resting filled quantity: " << marketengine.get_strategy().get_metrics().resting_filled_qty << "." << std::endl;
}

/**
    ============================================================
    TEST 2: SubmitSingleBuyOrder
    ============================================================
    PURPOSE: Verify a single buy order can be submitted and is correctly reflected in the order book.
    ============================================================
*/
TEST(MarketEngineTest, SubmitSingleBuyOrder) {
}

/**
    ============================================================
    TEST 3: SubmitSingleSellOrder
    ============================================================
    PURPOSE: Verify a single sell order can be submitted and is correctly reflected in the order book.
    ============================================================
*/
TEST(MarketEngineTest, SubmitSingleSellOrder) {
}

/**
    ============================================================
    TEST 4: SimpleMatchBuyAndSell
    ============================================================
    PURPOSE: Verify a simple buy and sell order at the same price are matched correctly.
    ============================================================
*/
TEST(MarketEngineTest, SimpleMatchBuyAndSell) {
}

/**
    ============================================================
    TEST 5: NoMatchDifferentPrices
    ============================================================
    PURPOSE: Verify that a buy and sell order with a price gap do not get matched.
    ============================================================
*/
TEST(MarketEngineTest, NoMatchDifferentPrices) {
}

/**
    ============================================================
    TEST 6: PartialFillMarketOrder
    ============================================================
    PURPOSE: Verify a larger market order is partially filled by a smaller limit order.
    ============================================================
*/
TEST(MarketEngineTest, PartialFillMarketOrder) {
}

/**
    ============================================================
    TEST 7: FullFillMultipleLimitOrders
    ============================================================
    PURPOSE: Verify a market order is fully filled by multiple smaller limit orders.
    ============================================================
*/
TEST(MarketEngineTest, FullFillMultipleLimitOrders) {
}

/**
    ============================================================
    TEST 8: CancelOrder
    ============================================================
    PURPOSE: Verify an existing order can be cancelled and is removed from the order book.
    ============================================================
*/
TEST(MarketEngineTest, CancelOrder) {
}

/**
    ============================================================
    TEST 9: CancelNonExistentOrder
    ============================================================
    PURPOSE: Verify that attempting to cancel a non-existent order has no effect.
    ============================================================
*/
TEST(MarketEngineTest, CancelNonExistentOrder) {
}

/**
    ============================================================
    TEST 10: MarketPriceCalculation
    ============================================================
    PURPOSE: Verify the market price (mid-price) is calculated correctly after new orders are added.
    ============================================================
*/
TEST(MarketEngineTest, MarketPriceCalculation) {
}