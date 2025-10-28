#include <gtest/gtest.h>
#include "../include/Metrics.h"

/**
    TEST 1: PositionTracking
    PURPOSE: Test if the position is tracked correctly when buy and sell orders are filled
*/
TEST(MetricsTest, PositionTracking) {
    Metrics metrics;
    
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 10, false);
    metrics.on_fill(1, 100, 1000, 10, false);
    
    EXPECT_EQ(metrics.get_position(), 10);

    metrics.on_order_placed(2, Metrics::Side::SELLS, 105, 1001, 5, false);
    metrics.on_fill(2, 105, 1001, 5, false);

    EXPECT_EQ(metrics.get_position(), 5);
}

/**
    TEST 2: AverageEntryPriceCalculation
    PURPOSE: Test weighted average entry price calculation
*/
TEST(MetricsTest, AverageEntryPriceCalculation) {
    Metrics metrics;
    
    
}

/**
    TEST 3: RealizedPnLCalculation
    PURPOSE: Test realized PnL is calculated correctly when closing positions
*/
TEST(MetricsTest, RealizedPnLCalculation) {
    Metrics metrics;
    
    
}

/**
    TEST 4: UnrealizedPnLCalculation
    PURPOSE: Test unrealized PnL updates with market price changes
*/
TEST(MetricsTest, UnrealizedPnLCalculation) {
    Metrics metrics;
    
    
}

/**
    TEST 5: TotalPnLCalculation
    PURPOSE: Test total PnL = realized + unrealized
*/
TEST(MetricsTest, TotalPnLCalculation) {
    Metrics metrics;
    
    
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