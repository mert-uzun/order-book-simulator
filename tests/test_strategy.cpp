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
    OrderBook orderbook;
    Strategy strategy(orderbook, 100, 2, 1000, 3, 500000);
    
    // ===== Quote Size =====
    EXPECT_NO_THROW(strategy.set_quote_size(500))
        << "set_quote_size throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_quote_size(), 500)
        << "Value of quote size does not match with the expected value after setting. Expected: 500, Result: " << strategy.get_quote_size() << std::endl;

    // ===== Tick Offset From Mid =====
    EXPECT_NO_THROW(strategy.set_tick_offset_from_mid(1))
        << "set_tick_offset_from_mid throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_tick_offset_from_mid(), 1)
        << "Value of tick offset from mid does not match with the expected value after setting. Expected: 1, Result: " << strategy.get_tick_offset_from_mid() << std::endl;

    // ===== Max Inventory =====
    EXPECT_NO_THROW(strategy.set_max_inventory(1500))
        << "set_max_inventory throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_max_inventory(), 1500)
        << "Value of max inventory does not match with the expected value after setting. Expected: 1500, Result: " << strategy.get_max_inventory() << std::endl;

    // ===== Cancel Threshold Ticks =====
    EXPECT_NO_THROW(strategy.set_cancel_threshold_ticks(4))
        << "set_cancel_threshold_ticks throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_cancel_threshold_ticks(), 4)
        << "Value of cancel threshold ticks does not match with the expected value after setting. Expected: 4, Result: " << strategy.get_cancel_threshold_ticks() << std::endl;

    // ===== Cooldown Between Requotes =====
    EXPECT_NO_THROW(strategy.set_cooldown_between_requotes(400000))
        << "set_cooldown_between_requotes throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_cooldown_between_requotes(), 400000)
        << "Value of cooldown between requotes does not match with the expected value after setting. Expected: 400000, Result: " << strategy.get_cooldown_between_requotes() << std::endl;

    // ===== Active Buy Order Id =====
    EXPECT_NO_THROW(strategy.set_active_buy_order_id(1001))
        << "set_active_buy_order_id throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_active_buy_order_id(), 1001)
        << "Value of active buy order id does not match with the expected value after setting. Expected: 1001, Result: " << strategy.get_active_buy_order_id() << std::endl;

    // ===== Active Sell Order Id =====
    EXPECT_NO_THROW(strategy.set_active_sell_order_id(1002))
        << "set_active_sell_order_id throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_active_sell_order_id(), 1002)
        << "Value of active sell order id does not match with the expected value after setting. Expected: 1002, Result: " << strategy.get_active_sell_order_id() << std::endl;

    // ===== Last Pinged Mid Price Ticks =====
    EXPECT_NO_THROW(strategy.set_last_pinged_mid_price_ticks(2000))
        << "set_last_pinged_mid_price_ticks throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_last_pinged_mid_price_ticks(), 2000)
        << "Value of last pinged mid price ticks does not match with the expected value after setting. Expected: 2000, Result: " << strategy.get_last_pinged_mid_price_ticks() << std::endl;

    // ===== Last Quote Time Us =====
    EXPECT_NO_THROW(strategy.set_last_quote_time_us(3000))
        << "set_last_quote_time_us throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_last_quote_time_us(), 3000)
        << "Value of last quote time us does not match with the expected value after setting. Expected: 3000, Result: " << strategy.get_last_quote_time_us() << std::endl;

    // ===== Latency Config =====
    EXPECT_NO_THROW(strategy.set_latency_config(100, 200, 300, 400, 500, 600, 700, 800, 900, 1000))
        << "set_latency_config throws an exception." << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_order_send_min(), 100)
        << "Value of order send min in latency queue settings does not match with the expected value after setting. Expected: 100, Result: " << strategy.get_latency_queue().get_order_send_min() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_order_send_max(), 200)
        << "Value of order send max in latency queue settings does not match with the expected value after setting. Expected: 200, Result: " << strategy.get_latency_queue().get_order_send_max() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_cancel_min(), 300)
        << "Value of cancel min in latency queue settings does not match with the expected value after setting. Expected: 300, Result: " << strategy.get_latency_queue().get_cancel_min() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_cancel_max(), 400)
        << "Value of cancel max in latency queue settings does not match with the expected value after setting. Expected: 400, Result: " << strategy.get_latency_queue().get_cancel_max() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_modify_min(), 500)
        << "Value of modify min in latency queue settings does not match with the expected value after setting. Expected: 500, Result: " << strategy.get_latency_queue().get_modify_min() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_modify_max(), 600)
        << "Value of modify max in latency queue settings does not match with the expected value after setting. Expected: 600, Result: " << strategy.get_latency_queue().get_modify_max() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_acknowledge_fill_min(), 700)
        << "Value of acknowledge fill min in latency queue settings does not match with the expected value after setting. Expected: 700, Result: " << strategy.get_latency_queue().get_acknowledge_fill_min() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_acknowledge_fill_max(), 800)
        << "Value of acknowledge fill max in latency queue settings does not match with the expected value after setting. Expected: 800, Result: " << strategy.get_latency_queue().get_acknowledge_fill_max() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_market_update_min(), 900)
        << "Value of market update min in latency queue settings does not match with the expected value after setting. Expected: 900, Result: " << strategy.get_latency_queue().get_market_update_min() << std::endl;
    EXPECT_EQ(strategy.get_latency_queue().get_market_update_max(), 1000)
        << "Value of market update max in latency queue settings does not match with the expected value after setting. Expected: 1000, Result: " << strategy.get_latency_queue().get_market_update_max() << std::endl;
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
    ============================================================
*/