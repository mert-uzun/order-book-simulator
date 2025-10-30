#include <gtest/gtest.h>
#include "../include/Strategy.h"

// Below are the default values of strategy parameters for this test suite
// quote_size = 100,
// tick_offset = 2,
// max_inv = 1000,
// cancel_threshold = 3,
// cooldown_between_requotes = 500000 -> 0.5 seconds

/**
    ============================================================
    TEST 1: ConstructorInitialization
    ============================================================
    PURPOSE: Verify Strategy initializes with correct parameters
    ============================================================
*/
TEST(StrategyTest, ConstructorInitialization) {
    OrderBook orderbook;
    Strategy strategy(orderbook, 100, 2, 1000, 3, 500000);

    EXPECT_EQ(strategy.get_quote_size(), 100)
        << "There is a problem with the quote size upon Strategy construction. Expected: 100, Result: " << strategy.get_quote_size() << std::endl;
    EXPECT_EQ(strategy.get_tick_offset_from_mid(), 2)
        << "There is a problem with the tick offset from mid upon Strategy construction. Expected: 2, Result: " << strategy.get_tick_offset_from_mid() << std::endl;
    EXPECT_EQ(strategy.get_max_inventory(), 1000)
        << "There is a problem with the max inventory upon Strategy construction. Expected: 1000, Result: " << strategy.get_max_inventory() << std::endl;
    EXPECT_EQ(strategy.get_cancel_threshold_ticks(), 3)
        << "There is a problem with the cancel threshold ticks upon Strategy construction. Expected: 3, Result: " << strategy.get_cancel_threshold_ticks() << std::endl;
    EXPECT_EQ(strategy.get_cooldown_between_requotes(), 500000)
        << "There is a problem with the cooldown between requotes upon Strategy construction. Expected: 500000, Result: " << strategy.get_cooldown_between_requotes() << std::endl;
    EXPECT_TRUE(strategy.get_buy_pongs().empty())
        << "Buy pongs are not empty after Strategy construction." << std::endl;
    EXPECT_TRUE(strategy.get_sell_pongs().empty())
        << "Sell pongs are not empty after Strategy construction." << std::endl;
    EXPECT_TRUE(strategy.get_latency_queue().is_empty())
        << "Latency queue is not empty after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_best_bid_ticks(), 0)
        << "Best bid ticks is not 0 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_best_ask_ticks(), 0)
        << "Best ask ticks is not 0 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_mid_price_ticks(), 0)
        << "Mid price ticks is not 0 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_current_market_price_ticks(), 0)
        << "Current market price ticks is not 0 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_spread_ticks(), 0)
        << "Spread ticks is not 0 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_current_inventory(), 0)
        << "Current inventory is not 0 after Strategy construction. (strategy.metrics.position)" << std::endl;
    EXPECT_EQ(strategy.get_active_buy_order_id(), -1)
        << "Active buy order id is not -1 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_active_sell_order_id(), -1)
        << "Active sell order id is not -1 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_last_pinged_mid_price_ticks(), 0)
        << "Last mid price ticks is not 0 after Strategy construction." << std::endl;
    EXPECT_EQ(strategy.get_last_quote_time_us(), 0)
        << "Last quote time us is not 0 after Strategy construction." << std::endl;
}

/**
    ============================================================
    TEST 2: SettersAndGetters
    ============================================================
    PURPOSE: Test setters update values correctly and getters return them
    ============================================================
*/
TEST(StrategyTest, SettersAndGetters) {
    
}

/**
    ============================================================
    TEST 3: PlacePingOrders
    ============================================================
    PURPOSE: Place ping buy and sell orders, verify they're added with correct prices
    ============================================================
*/
TEST(StrategyTest, PlacePingOrders) {
    
}

/**
    ============================================================
    TEST 4: InventoryLimitsRespected
    ============================================================
    PURPOSE: Verify strategy won't place orders that would exceed max_inventory
    ============================================================
*/
TEST(StrategyTest, InventoryLimitsRespected) {
    
}

/**
    ============================================================
    TEST 5: CooldownBetweenRequotes
    ============================================================
    PURPOSE: Verify strategy respects cooldown period and doesn't place orders too quickly
    ============================================================
*/
TEST(StrategyTest, CooldownBetweenRequotes) {
    
}

/**
    ============================================================
    TEST 6: CancelOnMarketMove
    ============================================================
    PURPOSE: When market moves beyond cancel_threshold, verify active orders get cancelled
    ============================================================
*/
TEST(StrategyTest, CancelOnMarketMove) {
    
}

/**
    ============================================================
    TEST 7: NoCancelWithinThreshold
    ============================================================
    PURPOSE: When market moves but stays within threshold, orders should NOT be cancelled
    ============================================================
*/
TEST(StrategyTest, NoCancelWithinThreshold) {
    
}

/**
    ============================================================
    TEST 8: PongOnPingBuyFill
    ============================================================
    PURPOSE: When a ping buy fills, verify pong sell order is placed at fill_price + 1