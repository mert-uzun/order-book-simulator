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
    
    // =============================== //
    // Initial state - no position
    // =============================== //
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 0)
        << "Unrealized PnL should be 0 initially.";
    

    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1001, 5, false);

    // =============================== //
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
    
    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);
    metrics.on_fill(2, 100, 1001, 5, false);

    // =============================== //
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

    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 110, 1004, 5, false);

    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, false);
    metrics.on_fill(6, 100, 1005, 5, false);

    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);
    metrics.on_fill(7, 100, 1006, 10, false);

    // =============================== //
    // Short → Long flip
    // =============================== //
    metrics.on_market_price_update(1007, 105, 107);

    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 5 * (106 - 100))
        << "Unrealized PnL should be 5 * (106 - 100) = 30 after position flips from negative to positive with avg entry price 100 and market price 106.";
}

/**
    TEST 5: TotalPnLCalculation
    PURPOSE: Test total PnL = realized + unrealized
*/
TEST(MetricsTest, TotalPnLCalculation) {
    Metrics metrics;
    metrics.set_config(0.001, 0, 0, Metrics::MarkingMethod::MID, 1000000);

    // ========================================
    // Order 1: Buy 5 @ 100
    // ========================================
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1000, 5, false);
    // Position: +5 @ avg entry 100
    
    metrics.on_market_price_update(1000, 100, 100);
    // Mark = 100, Unrealized PnL = 5 * (100 - 100) = 0
    // Realized PnL = 0, Total PnL = 0
    EXPECT_EQ(metrics.get_position(), 5) 
    << "Position should be 5 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
    << "Average entry price should be 100 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 0)
    << "Unrealized PnL should be 0 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
    << "Realized PnL should be 0 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 0)
    << "Total PnL should be 0 after buying 5 shares at 100.";

    // ========================================
    // Order 2: Buy 5 @ 100
    // ========================================
    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, false);
    metrics.on_fill(2, 100, 1001, 5, false);
    // Position: +10 @ avg entry 100
    
    metrics.on_market_price_update(1001, 102, 104);
    // Mark = 103, Unrealized PnL = 10 * (103 - 100) = +30
    // Realized PnL = 0, Total PnL = +30
    EXPECT_EQ(metrics.get_position(), 10)
    << "Position should be 10 after buying another 5 shares at 100.";
    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
    << "Average entry price should be 100 after buying another 5 shares at 100.";
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 30)
    << "Unrealized PnL should be 30 with 10 share position at mark price 103.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 0)
    << "Realized PnL should still be 0 after adding to long position.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 30)
    << "Total PnL should be 30 with 10 share position at mark price 103.";

    // ========================================
    // Order 3: Sell 5 @ 105 (Partial Close)
    // ========================================
    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 5, false);
    metrics.on_fill(3, 105, 1002, 5, false);
    // Position: +5 @ avg entry 100 (closes 5 long)
    // Realized PnL = 5 * (105 - 100) = +25
    
    metrics.on_market_price_update(1002, 104, 106);
    // Mark = 105, Unrealized PnL = 5 * (105 - 100) = +25
    // Realized PnL = +25, Total PnL = +50
    EXPECT_EQ(metrics.get_position(), 5)
    << "Position should be 5 after selling 5 shares at 105.";
    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
    << "Average entry price should be 100 after selling 5 shares at 105.";
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 25)
    << "Unrealized PnL should be 25 after selling 5 shares at 105.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 25)
    << "Realized PnL should be 25 after selling 5 shares at 105.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 50)
    << "Total PnL should be 50 after selling 5 shares at 105.";

    // ========================================
    // Order 4: Sell 10 @ 110 (Position Flip: Long → Short)
    // ========================================
    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, false);
    metrics.on_fill(4, 110, 1003, 10, false);
    // Position: -5 @ avg entry 110 (closes remaining 5 long, opens 5 short)
    // Realized PnL = +25 + 5 * (110 - 100) = +75
    
    metrics.on_market_price_update(1003, 108, 112);
    // Mark = 110, Unrealized PnL = -5 * (110 - 110) = 0
    // Realized PnL = +75, Total PnL = +75
    EXPECT_EQ(metrics.get_position(), -5)
    << "Position should be -5 after selling 10 shares at 110.";
    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 110)
    << "Average entry price should be 110 after selling 10 shares at 110.";
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 0)
    << "Unrealized PnL should be 0 after selling 10 shares at 110.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 75)
    << "Realized PnL should be 75 after selling 10 shares at 110.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 75)
    << "Total PnL should be 75 after selling 10 shares at 110.";

    // ========================================
    // Order 5: Sell 5 @ 110 (Add to Short)
    // ========================================
    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 110, 1004, 5, false);
    // Position: -10 @ avg entry 110
    
    metrics.on_market_price_update(1004, 106, 108);
    // Mark = 107, Unrealized PnL = -10 * (107 - 110) = -10 * (-3) = +30
    // Realized PnL = +75, Total PnL = +105
    EXPECT_EQ(metrics.get_position(), -10)
    << "Position should be -10 after selling 5 shares at 110.";
    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 110)
    << "Average entry price should be 110 after selling 5 shares at 110.";
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 30)
    << "Unrealized PnL should be 30 after selling 5 shares at 110.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 75)
    << "Realized PnL should be 75 after selling 5 shares at 110.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 105)
    << "Total PnL should be 105 after selling 5 shares at 110.";

    // ========================================
    // Order 6: Buy 5 @ 100 (Partial Close Short)
    // ========================================
    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, false);
    metrics.on_fill(6, 100, 1005, 5, false);
    // Position: -5 @ avg entry 110 (closes 5 short)
    // Realized PnL = +75 + 5 * (110 - 100) = +125
    
    metrics.on_market_price_update(1005, 99, 101);
    // Mark = 100, Unrealized PnL = -5 * (100 - 110) = -5 * (-10) = +50
    // Realized PnL = +125, Total PnL = +175
    EXPECT_EQ(metrics.get_position(), -5)
    << "Position should be -5 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 110)
    << "Average entry price should be 110 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 50)
    << "Unrealized PnL should be 50 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 125)
    << "Realized PnL should be 125 after buying 5 shares at 100.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 175)
    << "Total PnL should be 175 after buying 5 shares at 100.";

    // ========================================
    // Order 7: Buy 10 @ 100 (Position Flip: Short → Long)
    // ========================================
    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);
    metrics.on_fill(7, 100, 1006, 10, false);
    // Position: +5 @ avg entry 100 (closes remaining 5 short, opens 5 long)
    // Realized PnL = +125 + 5 * (110 - 100) = +175
    
    metrics.on_market_price_update(1006, 98, 102);
    // Mark = 100, Unrealized PnL = 5 * (100 - 100) = 0
    // Realized PnL = +175, Total PnL = +175
    EXPECT_EQ(metrics.get_position(), 5);
    EXPECT_EQ(metrics.get_avg_entry_price_ticks(), 100)
    << "Average entry price should be 100 after buying 10 shares at 100.";
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 0)
    << "Unrealized PnL should be 0 after buying 10 shares at 100.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 175)
    << "Realized PnL should be 175 after buying 10 shares at 100.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 175)
    << "Total PnL should be 175 after buying 10 shares at 100.";

    // ========================================
    // Final Market Update: Price Drops
    // ========================================
    metrics.on_market_price_update(1007, 94, 96);
    // Mark = 95, Unrealized PnL = 5 * (95 - 100) = -25
    // Realized PnL = +175, Total PnL = +150
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), -25)
    << "Unrealized PnL should be -25 after market price drops to 95.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 175)
    << "Realized PnL should be 175 after market price drops to 95.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 150)
    << "Total PnL should be 150 after market price drops to 95.";

    // ========================================
    // Final Market Update: Price Rises
    // ========================================
    metrics.on_market_price_update(1008, 114, 116);
    // Mark = 115, Unrealized PnL = 5 * (115 - 100) = +75
    // Realized PnL = +175, Total PnL = +250
    EXPECT_EQ(metrics.get_unrealized_pnl_ticks(), 75)
    << "Unrealized PnL should be 75 after market price rises to 115.";
    EXPECT_EQ(metrics.get_realized_pnl_ticks(), 175)
    << "Realized PnL should be 175 after market price rises to 115.";
    EXPECT_EQ(metrics.get_total_pnl_ticks(), 250)
    << "Total PnL should be 250 after market price rises to 115.";
}

/**
    TEST 6: FeeAccounting
    PURPOSE: Test maker rebates and taker fees are tracked correctly
*/
TEST(MetricsTest, FeeAccounting) {
    Metrics metrics;
    // Set maker_rebate = 2 ticks/share, taker_fee = 5 ticks/share
    metrics.set_config(0.001, 2, 5, Metrics::MarkingMethod::MID, 1000000);

    // ========================================
    // Order 1: Buy 5 @ 100 (RESTING - Maker Rebate)
    // ========================================
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 5, false);
    metrics.on_fill(1, 100, 1000, 5, false); // is_ioc = false
    // Fees = 0 - (2 * 5) = -10 (rebate earned)
    
    EXPECT_EQ(metrics.fees_ticks, -10)
        << "Fees should be -10 after earning maker rebate on 5 shares.";

    // ========================================
    // Order 2: Buy 5 @ 100 (IOC - Taker Fee)
    // ========================================
    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 5, true);
    metrics.on_fill(2, 100, 1001, 5, true); // is_ioc = true
    // Fees = -10 + (5 * 5) = +15 (paid taker fee)
    
    EXPECT_EQ(metrics.fees_ticks, 15)
        << "Fees should be 15 after paying taker fee on 5 shares.";

    // ========================================
    // Order 3: Sell 5 @ 105 (RESTING - Maker Rebate)
    // ========================================
    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 5, false);
    metrics.on_fill(3, 105, 1002, 5, false); // is_ioc = false
    // Fees = 15 - (2 * 5) = +5 (earned rebate)
    
    EXPECT_EQ(metrics.fees_ticks, 5)
        << "Fees should be 5 after earning maker rebate on 5 shares.";

    // ========================================
    // Order 4: Sell 10 @ 110 (IOC - Taker Fee)
    // ========================================
    metrics.on_order_placed(4, Metrics::Side::SELLS, 110, 1003, 10, true);
    metrics.on_fill(4, 110, 1003, 10, true); // is_ioc = true
    // Fees = 5 + (5 * 10) = +55 (paid taker fee)
    
    EXPECT_EQ(metrics.fees_ticks, 55)
        << "Fees should be 55 after paying taker fee on 10 shares.";

    // ========================================
    // Order 5: Sell 5 @ 110 (RESTING - Maker Rebate)
    // ========================================
    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1004, 5, false);
    metrics.on_fill(5, 110, 1004, 5, false); // is_ioc = false
    // Fees = 55 - (2 * 5) = +45 (earned rebate)
    
    EXPECT_EQ(metrics.fees_ticks, 45)
        << "Fees should be 45 after earning maker rebate on 5 shares.";

    // ========================================
    // Order 6: Buy 5 @ 100 (IOC - Taker Fee)
    // ========================================
    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1005, 5, true);
    metrics.on_fill(6, 100, 1005, 5, true); // is_ioc = true
    // Fees = 45 + (5 * 5) = +70 (paid taker fee)
    
    EXPECT_EQ(metrics.fees_ticks, 70)
        << "Fees should be 70 after paying taker fee on 5 shares.";

    // ========================================
    // Order 7: Buy 10 @ 100 (RESTING - Maker Rebate)
    // ========================================
    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1006, 10, false);
    metrics.on_fill(7, 100, 1006, 10, false); // is_ioc = false
    // Fees = 70 - (2 * 10) = +50 (earned rebate)
    
    EXPECT_EQ(metrics.fees_ticks, 50)
        << "Fees should be 50 after earning maker rebate on 10 shares.";

    // ========================================
    // Summary: Total Fee Breakdown
    // ========================================
    // Total maker rebates earned: 5+5+5+10 = 25 shares × 2 = -50 ticks
    // Total taker fees paid: 5+10+5 = 20 shares × 5 = +100 ticks
    // Net fees paid: 100 - 50 = +50 ticks
    
    EXPECT_EQ(metrics.fees_ticks, 50)
        << "Final fees should be 50 ticks (net cost after rebates).";
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