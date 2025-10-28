#include <gtest/gtest.h>
#include "../include/Metrics.h"

/**
    TEST 1: 
    PURPOSE: Test if the position is tracked correctly when buy and sell orders are filled, considers reducing, increasing, and flipping positions
*/
TEST(MetricsTest, PositionTracking) {
    Metrics metrics;

    EXPECT_EQ(metrics.get_position(), 0)
        << "Position should be 0 initially.";
    
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1000, 5, false);

    EXPECT_EQ(metrics.get_position(), 10)
        << "Position should be 5 after buying 5 shares at 100 when position was 0.";

    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);
    metrics.on_fill(2, 100, 1001, 5, false);

    
    EXPECT_EQ(metrics.get_position(), 10)
        << "Position should be 10 after buying 5 shares at 100 when position was 5.";

    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 5, false);
    metrics.on_fill(3, 105, 1002, 5, false);

    EXPECT_EQ(metrics.get_position(), 5)
        << "Position should be 5 after selling 5 shares at 105 when position was 10.";

    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, false);
    metrics.on_fill(4, 110, 1003, 10, false);

    EXPECT_EQ(metrics.get_position(), -5)
        << "Position should be -5 after selling 10 shares at 110 when position was 5.";

    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 110, 1004, 5, false);

    EXPECT_EQ(metrics.get_position(), -10)
        << "Position should be -10 after selling 5 shares at 110 when position was -5.";

    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, false);
    metrics.on_fill(6, 100, 1005, 5, false);

    EXPECT_EQ(metrics.get_position(), -5)
        << "Position should be -5 after buying 5 shares at 100 when position was -10.";
    
    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);
    metrics.on_fill(7, 100, 1006, 10, false);

    EXPECT_EQ(metrics.get_position(), 5)
        << "Position should be 5 after buying 10 shares at 100 when position was -5.";

}

/**
    TEST 2: AverageEntryPriceCalculation
    PURPOSE: Test weighted average entry price calculation
*/
TEST(MetricsTest, AverageEntryPriceCalculation) {
    Metrics metrics;

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 0)
        << "Average entry price should be 0 initially.";

    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1000, 5, false);

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
        << "Average entry price should be 100 after buying 5 shares at 100 when average entry price was 0 for position 0.";

    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);
    metrics.on_fill(2, 100, 1001, 5, false);

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
        << "Average entry price should stay 100 after buying 5 shares at 100 when average entry price was 100 for position 5.";

    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 5, false);
    metrics.on_fill(3, 105, 1002, 5, false);

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
        << "Sell shouldn't change average entry price when position is positive.";

    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, false);
    metrics.on_fill(4, 110, 1003, 10, false);

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 110)
        << "Average entry price should adjust to new average value when flipping position from positive to negative.";

    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 115, 1004, 5, false);

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 112.5)
        << "Sell when position is negative should adjust average entry price to new average value.";

    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, false);
    metrics.on_fill(6, 100, 1005, 5, false);

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 112.5)
        << "Buy shouldn't change average entry price when position is negative.";

    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);
    metrics.on_fill(7, 100, 1006, 10, false);

    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
        << "Average entry price should adjust to new average value when flipping position from negative to positive.";
}

/**
    TEST 3: RealizedPnLCalculation
    PURPOSE: Test realized PnL is calculated correctly when closing positions
*/
TEST(MetricsTest, RealizedPnLCalculation) {
    Metrics metrics;

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
        << "Realized PnL should be 0 initially.";
    
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
        << "Realized PnL doesn't change when order is placed but not filled.";
    
    metrics.on_fill(1, 100, 1000, 5, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
        << "Realized PnL should be 0 after buying 5 shares at 100 when realized PnL was 0 for position 0 with avg entry price 0.";

    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(2, 100, 1001, 5, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
        << "Realized PnL should be 0 after buying 5 shares at 100 when realized PnL was 0 for position 5 with avg entry price 100.";

    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 3, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(3, 105, 1002, 3, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 15)
        << "Realized PnL should be 15 after selling 3 shares at 105 when realized PnL was 0 for position 10 with avg entry price 100.";

    metrics.on_order_placed(31, Metrics::Side::SELLS, 95, 1002, 2, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 15)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(31, 95, 1002, 2, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 5)
        << "Realized PnL should be 5 after selling 2 shares at 95 when realized PnL was 15 for position 7 with avg entry price 100.";

    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 5)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(4, 110, 1003, 10, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 55)
        << "Realized PnL should be 55 after selling 10 shares at 110 when realized PnL was 5 for position 5 with avg entry price 100.";

    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 55)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(5, 110, 1004, 5, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 55)
        << "Realized PnL should be 55 after selling 5 shares at 110 when realized PnL was 55 for position -5 with avg entry price 110.";

    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 3, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 55)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(6, 100, 1005, 3, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 85)
        << "Realized PnL should be 85 after buying 3 shares at 100 when realized PnL was 55 for position -10 with avg entry price 110.";

    metrics.on_order_placed(61, Metrics::Side::BUYS, 120, 1005, 2, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 85)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(61, 120, 1005, 2, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 65)
        << "Realized PnL should be 65 after buying 2 shares at 120 when realized PnL was 85 for position -7 with avg entry price 110.";

    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 65)
        << "Realized PnL doesn't change when order is placed but not filled.";

    metrics.on_fill(7, 100, 1006, 10, false);

    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 115)
        << "Realized PnL should be 115 after buying 10 shares at 100 when realized PnL was 65 for position -5 with avg entry price 110.";
}

/**
    TEST 4: UnrealizedPnLCalculation
    PURPOSE: Test unrealized PnL updates with market price changes
*/
TEST(MetricsTest, UnrealizedPnLCalculation) {
    Metrics metrics;
    metrics.set_config(0.001, 0, 0, Metrics::MarkingMethod::MID, 1000000);

    // Initial state - no position
    // =============================== //
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 0)
        << "Unrealized PnL should be 0 initially.";
    // =============================== //
    
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1001, 5, false);

    // Long position profit
    // =============================== //
    metrics.on_market_price_update(1002, 105, 107);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 5 * (106 - 100))
        << "Unrealized PnL should be 5 * (106 - 100) = 30 after buying 5 shares at 100 and market price is 106.";
    // =============================== //

    // Long position loss
    // =============================== //
    metrics.on_market_price_update(1003, 90, 100);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 5 * (95 - 100))
        << "Unrealized PnL should be 5 * (95 - 100) = -25 after market price moves to 95 from 100.";
    // =============================== //
    
    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);
    metrics.on_fill(2, 100, 1001, 5, false);

    // Partial close of long position
    // =============================== //
    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 5, false);
    metrics.on_fill(3, 105, 1002, 5, false);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 5 * (95 - 100))
        << "Unrealized PnL should be 5 * (95 - 100) = -25 after selling 5 shares at market price 95.";
    // =============================== //

    // Long → Short flip
    // =============================== //
    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, false);
    metrics.on_fill(4, 110, 1003, 10, false);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), -5 * (95 - 110))
        << "Unrealized PnL should be -5 * (95 - 110) = 75 after flipping position to -5 shares with avg entry price 110 when market price is 95.";
    // =============================== //

    // Short position profit
    // =============================== //
    metrics.on_market_price_update(1004, 105, 107);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), -5 * (106 - 110))
        << "Unrealized PnL should be -5 * (106 - 110) = 20 after market price moves to 106.";
    // =============================== //

    // Short position loss
    // =============================== //
    metrics.on_market_price_update(1005, 110, 120);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), -5 * (115 - 110))
        << "Unrealized PnL should be -5 * (115 - 110) = -25 after market price moves to 115.";
    // =============================== //

    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 110, 1004, 5, false);

    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, false);
    metrics.on_fill(6, 100, 1005, 5, false);

    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);
    metrics.on_fill(7, 100, 1006, 10, false);

    // Short → Long flip
    // =============================== //
    metrics.on_market_price_update(1007, 105, 107);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 5 * (106 - 100))
        << "Unrealized PnL should be 5 * (106 - 100) = 30 after position flips from negative to positive with avg entry price 100 and market price 106.";
    // =============================== //
}

/**
    TEST 5: TotalPnLCalculation
    PURPOSE: Test total PnL = realized + unrealized
*/
TEST(MetricsTest, TotalPnLCalculation) {
    Metrics metrics;

    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1000, 5, false);

    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);
    metrics.on_fill(2, 100, 1001, 5, false);

    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 5, false);
    metrics.on_fill(3, 105, 1002, 5, false);

    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, false);
    metrics.on_fill(4, 110, 1003, 10, false);

    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 110, 1004, 5, false);

    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, false);
    metrics.on_fill(6, 100, 1005, 5, false);
    
    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);
    metrics.on_fill(7, 100, 1006, 10, false);
}

/**
    TEST 6: FeeAccounting
    PURPOSE: Test maker rebates and taker fees are tracked correctly
*/
TEST(MetricsTest, FeeAccounting) {
    Metrics metrics;
    
    
}

/**
    TEST 7: FillRatioCalculation
    PURPOSE: Test fill ratio = resting_filled / resting_attempted
*/
TEST(MetricsTest, FillRatioCalculation) {
    Metrics metrics;
    
    
}

/**
    TEST 8: SharpeRatioCalculation
    PURPOSE: Test Sharpe ratio calculation with return series
*/
TEST(MetricsTest, SharpeRatioCalculation) {
    Metrics metrics;
    
    
}

/**
    TEST 9: VolatilityCalculation
    PURPOSE: Test volatility = standard deviation of returns
*/
TEST(MetricsTest, VolatilityCalculation) {
    Metrics metrics;
    
    
}

/**
    TEST 10: MaxDrawdownCalculation
    PURPOSE: Test max drawdown = largest peak-to-trough decline
*/
TEST(MetricsTest, MaxDrawdownCalculation) {
    Metrics metrics;
    
    
}