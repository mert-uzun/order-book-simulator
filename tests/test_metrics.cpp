#include <gtest/gtest.h>
#include "../include/Metrics.h"

/**
    TEST 1: 
    PURPOSE: Test if the position is tracked correctly when buy and sell orders are filled, considers reducing, increasing, and flipping positions
*/
TEST(MetricsTest, PositionTracking) {
    Metrics metrics;

    EXPECT_EQ(metrics.get_position(), 0)
        << "Position should be 0 initially";
    
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1000, 5, false);

    EXPECT_EQ(metrics.get_position(), 10)
        << "Position should be 5 after buying 5 shares at 100 when position was 0";

    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);
    metrics.on_fill(2, 100, 1001, 5, false);

    
    EXPECT_EQ(metrics.get_position(), 10)
        << "Position should be 10 after buying 5 shares at 100 when position was 5";

    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 5, false);
    metrics.on_fill(3, 105, 1002, 5, false);

    EXPECT_EQ(metrics.get_position(), 5)
        << "Position should be 5 after selling 5 shares at 105 when position was 10";

    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, false);
    metrics.on_fill(4, 110, 1003, 10, false);

    EXPECT_EQ(metrics.get_position(), -5)
        << "Position should be -5 after selling 10 shares at 110 when position was 5";

    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 110, 1004, 5, false);

    EXPECT_EQ(metrics.get_position(), -10)
        << "Position should be -10 after selling 5 shares at 110 when position was -5";

    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, false);
    metrics.on_fill(6, 100, 1005, 5, false);

    EXPECT_EQ(metrics.get_position(), -5)
        << "Position should be -5 after buying 5 shares at 100 when position was -10";
}

/**
    TEST 2: AverageEntryPriceCalculation
    PURPOSE: Test weighted average entry price calculation
*/
TEST(MetricsTest, AverageEntryPriceCalculation) {
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
    
}

/**
    TEST 3: RealizedPnLCalculation
    PURPOSE: Test realized PnL is calculated correctly when closing positions
*/
TEST(MetricsTest, RealizedPnLCalculation) {
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
}

/**
    TEST 4: UnrealizedPnLCalculation
    PURPOSE: Test unrealized PnL updates with market price changes
*/
TEST(MetricsTest, UnrealizedPnLCalculation) {
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