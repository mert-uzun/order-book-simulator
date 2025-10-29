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
    metrics.set_config(0.001, 0, 0, Metrics::MarkingMethod::MID, 1000000);

    // ========================================
    // Case 1: No Orders Yet - Division by Zero Protection
    // ========================================
    EXPECT_EQ(metrics.get_fill_ratio(), 0.0)
        << "Fill ratio should be 0 when no resting orders have been placed.";

    // ========================================
    // Case 2: IOC Orders Don't Count Toward Fill Ratio
    // ========================================
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000, 10, true); // IOC
    metrics.on_fill(1, 100, 1000, 10, true);
    
    EXPECT_EQ(metrics.get_fill_ratio(), 0.0)
        << "Fill ratio should remain 0 as IOC orders don't count.";

    // ========================================
    // Case 3: Resting Order Fully Filled
    // ========================================
    metrics.on_order_placed(2, Metrics::Side::BUYS, 100, 1001, 20, false); // Resting
    metrics.on_fill(2, 100, 1001, 20, false);
    // resting_attempted = 20, resting_filled = 20
    // Fill ratio = 20/20 = 1.0
    
    EXPECT_EQ(metrics.get_fill_ratio(), 1.0)
        << "Fill ratio should be 1.0 when all resting orders are filled.";

    // ========================================
    // Case 4: Resting Order Partially Filled Then Cancelled
    // ========================================
    metrics.on_order_placed(3, Metrics::Side::SELLS, 105, 1002, 30, false); // Resting
    metrics.on_fill(3, 105, 1002, 10, false); // Partial fill: 10 of 30
    metrics.on_order_cancelled(3, 1003); // Cancel remaining 20
    // resting_attempted = 20 + 30 = 50
    // resting_filled = 20 + 10 = 30
    // Fill ratio = 30/50 = 0.6
    
    EXPECT_DOUBLE_EQ(metrics.get_fill_ratio(), 0.6)
        << "Fill ratio should be 0.6 (30 filled out of 50 attempted).";

    // ========================================
    // Case 5: Multiple Fills on Same Order
    // ========================================
    metrics.on_order_placed(4, Metrics::Side::BUYS, 100, 1004, 40, false); // Resting
    metrics.on_fill(4, 100, 1005, 15, false); // First partial fill
    metrics.on_fill(4, 100, 1006, 15, false); // Second partial fill
    metrics.on_fill(4, 100, 1007, 10, false); // Final fill (total = 40)
    // resting_attempted = 50 + 40 = 90
    // resting_filled = 30 + 15 + 15 + 10 = 70
    // Fill ratio = 70/90 = 0.777...
    
    EXPECT_NEAR(metrics.get_fill_ratio(), 0.777777, 0.00001)
        << "Fill ratio should be ~0.7778 after multiple partial fills.";

    // ========================================
    // Case 6: Resting Order Cancelled Without Any Fill
    // ========================================
    metrics.on_order_placed(5, Metrics::Side::SELLS, 110, 1008, 25, false); // Resting
    metrics.on_order_cancelled(5, 1009); // Cancel without fill
    // resting_attempted = 90 + 25 = 115
    // resting_filled = 70 (unchanged)
    // Fill ratio = 70/115 = 0.608...
    
    EXPECT_NEAR(metrics.get_fill_ratio(), 0.608696, 0.00001)
        << "Fill ratio should drop to ~0.6087 after cancelled order.";

    // ========================================
    // Case 7: Mix of IOC and Resting
    // ========================================
    metrics.on_order_placed(6, Metrics::Side::BUYS, 100, 1010, 50, true); // IOC - doesn't count
    metrics.on_fill(6, 100, 1010, 50, true);
    
    metrics.on_order_placed(7, Metrics::Side::BUYS, 100, 1011, 35, false); // Resting
    metrics.on_fill(7, 100, 1011, 35, false);
    // resting_attempted = 115 + 35 = 150
    // resting_filled = 70 + 35 = 105
    // Fill ratio = 105/150 = 0.7
    
    EXPECT_DOUBLE_EQ(metrics.get_fill_ratio(), 0.7)
        << "Fill ratio should be 0.7 (IOC orders don't affect ratio).";

    // ========================================
    // Summary Check
    // ========================================
    // Total resting attempted: 20 + 30 + 40 + 25 + 35 = 150
    // Total resting filled: 20 + 10 + 40 + 0 + 35 = 105
    // IOC orders: 10 + 50 = 60 (not counted)
    EXPECT_EQ(metrics.get_fill_ratio(), 0.7)
        << "Final fill ratio should be: 70% of resting orders filled.";
}

/**
    TEST 8: SharpeRatioCalculation
    PURPOSE: Test Sharpe ratio calculation with return series
*/
TEST(MetricsTest, SharpeRatioCalculation) {
    Metrics metrics;
    // Set return_bucket_interval = 1,000,000 us (1 second)
    metrics.set_config(0.001, 0, 0, Metrics::MarkingMethod::MID, 1000000);

    // ========================================
    // Scenario: 6 return buckets with known returns
    // ========================================
    // Create returns: [0, 100, 200, 150, -50, 100]
    // Mean = (0+100+200+150-50+100)/6 = 500/6 = 83.333
    // Deviations: [-83.333, 16.667, 116.667, 66.667, -133.333, 16.667]
    // Squared: [6944.44, 277.78, 13611.11, 4444.44, 17777.78, 277.78]
    // Variance = 43333.33 / 6 = 7222.22
    // Std Dev = sqrt(7222.22) ≈ 84.98
    // Raw Sharpe = 83.333 / 84.98 ≈ 0.9806
    
    // ========================================
    // Bucket 0: Initial bucket (PnL = 0)
    // ========================================
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000000, 10, false);
    metrics.on_fill(1, 100, 1000000, 10, false);
    metrics.on_market_price_update(1000000, 100, 100);
    // Total PnL = 0, Bucket 0 return = 0 - 0 = 0
    
    // ========================================
    // Bucket 1: PnL goes from 0 to 100
    // ========================================
    metrics.on_order_placed(2, Metrics::Side::SELLS, 110, 2000000, 10, false);
    metrics.on_fill(2, 110, 2000000, 10, false);
    // Realized PnL = 10 * (110 - 100) = +100
    metrics.on_market_price_update(2000000, 110, 110); // Trigger bucket (2s passed)
    // Bucket 1 return = 100 - 0 = +100

    // ========================================
    // Bucket 2: PnL goes from 100 to 300
    // ========================================
    metrics.on_order_placed(3, Metrics::Side::BUYS, 110, 2500000, 20, false);
    metrics.on_fill(3, 110, 2500000, 20, false);
    
    metrics.on_order_placed(4, Metrics::Side::SELLS, 120, 3000000, 20, false);
    metrics.on_fill(4, 120, 3000000, 20, false);
    // Realized PnL = 100 + 20*(120-110) = 100 + 200 = 300
    metrics.on_market_price_update(3000000, 120, 120); // Trigger bucket (3s)
    // Bucket 2 return = 300 - 100 = +200

    // ========================================
    // Bucket 3: PnL goes from 300 to 450
    // ========================================
    metrics.on_order_placed(5, Metrics::Side::BUYS, 120, 3500000, 15, false);
    metrics.on_fill(5, 120, 3500000, 15, false);
    metrics.on_order_placed(6, Metrics::Side::SELLS, 130, 4000000, 15, false);
    metrics.on_fill(6, 130, 4000000, 15, false);
    // Realized PnL = 300 + 15*(130-120) = 300 + 150 = 450
    metrics.on_market_price_update(4000000, 130, 130); // Trigger bucket (4s)
    // Bucket 3 return = 450 - 300 = +150

    // ========================================
    // Bucket 4: PnL goes from 450 to 400 (LOSS)
    // ========================================
    metrics.on_order_placed(7, Metrics::Side::BUYS, 130, 4500000, 10, false);
    metrics.on_fill(7, 130, 4500000, 10, false);
    metrics.on_order_placed(8, Metrics::Side::SELLS, 125, 5000000, 10, false);
    metrics.on_fill(8, 125, 5000000, 10, false);
    // Realized PnL = 450 + 10*(125-130) = 450 - 50 = 400
    metrics.on_market_price_update(5000000, 125, 125); // Trigger bucket (5s)
    // Bucket 4 return = 400 - 450 = -50

    // ========================================
    // Bucket 5: PnL goes from 400 to 500
    // ========================================
    metrics.on_order_placed(9, Metrics::Side::BUYS, 125, 5500000, 10, false);
    metrics.on_fill(9, 125, 5500000, 10, false);
    metrics.on_order_placed(10, Metrics::Side::SELLS, 135, 6000000, 10, false);
    metrics.on_fill(10, 135, 6000000, 10, false);
    // Realized PnL = 400 + 10*(135-125) = 400 + 100 = 500
    metrics.on_market_price_update(6000000, 135, 135); // Trigger bucket (6s)
    // Bucket 5 return = 500 - 400 = +100

    // ========================================
    // Finalize to Calculate Sharpe Ratio
    // ========================================
    metrics.finalize(6000000);

    // Expected calculation:
    // Returns: [0, 100, 200, 150, -50, 100]
    // Mean = 500 / 6 = 83.333
    // Variance = 43333.33 / 6 = 7222.22
    // Std Dev = sqrt(7222.22) ≈ 84.98
    // Raw Sharpe = 83.333 / 84.98 ≈ 0.9806
    
    // Annualization factor:
    // Trading year = 252 days * 6.5 hours * 3600 sec * 1000000 us = 5,875,200,000,000 us
    // Buckets per year = 5,875,200,000,000 / 1,000,000 = 5,875,200
    // Scaling factor = sqrt(5,875,200) ≈ 2423.67
    // Annualized Sharpe = 0.9806 * 2423.67 ≈ 2376.7

    double expected_sharpe = (500.0 / 6.0) / std::sqrt(43333.33 / 6.0) * std::sqrt(5875200.0);
    
    EXPECT_NEAR(metrics.get_sharpe_ratio(), expected_sharpe, 1.0)
        << "Sharpe ratio should be approximately " << expected_sharpe;

    // Additional sanity checks
    EXPECT_GT(metrics.get_sharpe_ratio(), 0)
        << "Sharpe ratio should be positive for profitable trading with one loss bucket.";
}

/**
    TEST 9: VolatilityCalculation
    PURPOSE: Test volatility = standard deviation of returns
*/
TEST(MetricsTest, VolatilityCalculation) {
    Metrics metrics;
    metrics.set_config(0.001, 0, 0, Metrics::MarkingMethod::MID, 1000000);

    // ========================================
    // Create return series: [0, 100, 200, 150, -50, 100]
    // Expected volatility = std dev = 84.98
    // ========================================
    
    // Bucket 0: PnL = 0
    metrics.on_order_placed(1, Metrics::Side::BUYS, 100, 1000000, 10, false);
    metrics.on_fill(1, 100, 1000000, 10, false);
    metrics.on_market_price_update(1000000, 100, 100);
    
    // Bucket 1: PnL = 100 (return = +100)
    metrics.on_order_placed(2, Metrics::Side::SELLS, 110, 2000000, 10, false);
    metrics.on_fill(2, 110, 2000000, 10, false);
    metrics.on_market_price_update(2000000, 110, 110);
    
    // Bucket 2: PnL = 300 (return = +200)
    metrics.on_order_placed(3, Metrics::Side::BUYS, 110, 2500000, 20, false);
    metrics.on_fill(3, 110, 2500000, 20, false);
    metrics.on_order_placed(4, Metrics::Side::SELLS, 120, 3000000, 20, false);
    metrics.on_fill(4, 120, 3000000, 20, false);
    metrics.on_market_price_update(3000000, 120, 120);
    
    // Bucket 3: PnL = 450 (return = +150)
    metrics.on_order_placed(5, Metrics::Side::BUYS, 120, 3500000, 15, false);
    metrics.on_fill(5, 120, 3500000, 15, false);
    metrics.on_order_placed(6, Metrics::Side::SELLS, 130, 4000000, 15, false);
    metrics.on_fill(6, 130, 4000000, 15, false);
    metrics.on_market_price_update(4000000, 130, 130);
    
    // Bucket 4: PnL = 400 (return = -50)
    metrics.on_order_placed(7, Metrics::Side::BUYS, 130, 4500000, 10, false);
    metrics.on_fill(7, 130, 4500000, 10, false);
    metrics.on_order_placed(8, Metrics::Side::SELLS, 125, 5000000, 10, false);
    metrics.on_fill(8, 125, 5000000, 10, false);
    metrics.on_market_price_update(5000000, 125, 125);
    
    // Bucket 5: PnL = 500 (return = +100)
    metrics.on_order_placed(9, Metrics::Side::BUYS, 125, 5500000, 10, false);
    metrics.on_fill(9, 125, 5500000, 10, false);
    metrics.on_order_placed(10, Metrics::Side::SELLS, 135, 6000000, 10, false);
    metrics.on_fill(10, 135, 6000000, 10, false);
    metrics.on_market_price_update(6000000, 135, 135);

    // Finalize to calculate volatility
    metrics.finalize(6000000);

    // Expected volatility calculation:
    // Returns: [0, 100, 200, 150, -50, 100]
    // Mean = 500 / 6 = 83.333
    // Deviations from mean: [-83.333, 16.667, 116.667, 66.667, -133.333, 16.667]
    // Squared deviations: [6944.44, 277.78, 13611.11, 4444.44, 17777.78, 277.78]
    // Variance = 43333.33 / 6 = 7222.22
    // Volatility (Std Dev) = sqrt(7222.22) = 84.98
    
    double expected_volatility = std::sqrt(43333.33 / 6.0);
    
    EXPECT_NEAR(metrics.get_volatility(), expected_volatility, 0.01)
        << "Volatility should be approximately " << expected_volatility << ", but result is " << metrics.get_volatility();

    // Additional checks
    EXPECT_GT(metrics.get_volatility(), 0)
        << "Volatility should be positive for non-constant returns, but result is " << metrics.get_volatility();
}

/**
    TEST 10: MaxDrawdownCalculation
    PURPOSE: Test max drawdown = largest peak-to-trough decline
*/
TEST(MetricsTest, MaxDrawdownCalculation) {
    Metrics metrics;
    
    
}