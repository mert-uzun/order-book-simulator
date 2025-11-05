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
    EXPECT_EQ(marketengine.get_market_price_ticks(), 1000)
        << "The market price should be 1000 after initialization. Current market price: " << marketengine.get_market_price_ticks() << "." << std::endl;
    EXPECT_EQ(marketengine.get_spread(), 2)
        << "The spread should be 2 after initialization. Current spread: " << marketengine.get_spread() << "." << std::endl;
    EXPECT_EQ(marketengine.get_volatility(), 1.0)
        << "The volatility should be 1.0 after initialization. Current volatility: " << marketengine.get_volatility() << "." << std::endl;
    EXPECT_EQ(marketengine.get_fill_probability(), 0.3)
        << "The fill probability should be 0.3 after initialization. Current fill probability: " << marketengine.get_fill_probability() << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_buys().size(), 0)
        << "There should be no buy orders in the orderbook after initialization. Current size: " << marketengine.get_orderbook().get_buys().size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_sells().size(), 0)
        << "There should be no sell orders in the orderbook after initialization. Current size: " << marketengine.get_orderbook().get_sells().size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_active_buy_order_id(), -1)
        << "There should be no active buy order after initialization. Current active buy order id: " << marketengine.get_strategy().get_active_buy_order_id() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_active_sell_order_id(), -1)
        << "There should be no active sell order after initialization. Current active sell order id: " << marketengine.get_strategy().get_active_sell_order_id() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_tick_offset_from_mid(), 2)
        << "The tick offset from mid should be 2 after initialization. Current tick offset from mid: " << marketengine.get_strategy().get_tick_offset_from_mid() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_max_inventory(), 1000)
        << "The max inventory should be 1000 after initialization. Current max inventory: " << marketengine.get_strategy().get_max_inventory() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_cancel_threshold_ticks(), 3)
        << "The cancel threshold ticks should be 3 after initialization. Current cancel threshold ticks: " << marketengine.get_strategy().get_cancel_threshold_ticks() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_cooldown_between_requotes(), 500000)
        << "The cooldown between requotes should be 500000 after initialization. Current cooldown between requotes: " << marketengine.get_strategy().get_cooldown_between_requotes() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_quote_size(), 100)
        << "The quote size should be 100 after initialization. Current quote size: " << marketengine.get_strategy().get_quote_size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().order_cache.size(), 0)
        << "There should be no orders in the order cache after initialization. Current order cache size: " << marketengine.get_strategy().get_metrics().order_cache.size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().gross_traded_qty, 0)
        << "There should be no trades executed after initialization. Current gross traded quantity: " << marketengine.get_strategy().get_metrics().gross_traded_qty << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_trade_price_ticks, 0)
        << "There should be no last trade price after initialization. Current last trade price: " << marketengine.get_strategy().get_metrics().last_trade_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_mark_price_ticks, 0)
        << "There should be no last mark price after initialization before observing the market in strategy. Current last mark price: " << marketengine.get_strategy().get_metrics().last_mark_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_return_bucket_start_us, 0)
        << "There should be no last return bucket start time after initialization. Current last return bucket start time: " << marketengine.get_strategy().get_metrics().last_return_bucket_start_us << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().last_return_bucket_total_pnl_ticks, 0)
        << "There should be no last return bucket total PnL after initialization. Current last return bucket total PnL: " << marketengine.get_strategy().get_metrics().last_return_bucket_total_pnl_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().current_best_bid_price_ticks, 0)
        << "There should be no current best bid price after initialization. Current current best bid price: " << marketengine.get_strategy().get_metrics().current_best_bid_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().current_best_ask_price_ticks, 0)
        << "There should be no current best ask price after initialization. Current current best ask price: " << marketengine.get_strategy().get_metrics().current_best_ask_price_ticks << "." << std::endl;
    EXPECT_EQ(marketengine.get_strategy().get_metrics().volatility, 0.0)
        << "There should be no final volatility after initialization. Current volatility: " << marketengine.get_strategy().get_metrics().volatility << "." << std::endl;
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
    MarketEngine marketengine(100, 2, 1000, 3, 0, 1000, 2, 1.0, 0.3);
    marketengine.get_strategy().observe_the_market(1000, 1000);
    marketengine.execute_events_until(1500);
    marketengine.get_strategy().place_ping_buy(2000);
    marketengine.execute_events_until(2500);
    EXPECT_EQ(marketengine.get_orderbook().get_buys().size(), 1)
        << "There should be one buy order in the orderbook after submitting a buy order. Current size: " << marketengine.get_orderbook().get_buys().size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_buys().begin()->second.front().priceTick, marketengine.get_market_price_ticks() - 2)
        << "The buy order price tick should be 1500 after submitting a buy order. Current price tick: " << marketengine.get_orderbook().get_buys().begin()->second.front().priceTick << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_buys().begin()->second.front().quantity, 100)
        << "The buy order quantity should be 100 after submitting a buy order. Current quantity: " << marketengine.get_orderbook().get_buys().begin()->second.front().quantity << "." << std::endl;
}

/**
    ============================================================
    TEST 3: SubmitSingleSellOrder
    ============================================================
    PURPOSE: Verify a single sell order can be submitted and is correctly reflected in the order book.
    ============================================================
*/
TEST(MarketEngineTest, SubmitSingleSellOrder) {
    MarketEngine marketengine(100, 2, 1000, 3, 0, 1000, 2, 1.0, 0.3);
    marketengine.get_strategy().observe_the_market(1000, 1000);
    marketengine.execute_events_until(1500);
    marketengine.get_strategy().place_ping_ask(2000);
    marketengine.execute_events_until(2500);
    EXPECT_EQ(marketengine.get_orderbook().get_sells().size(), 1)
        << "There should be one sell order in the orderbook after submitting a sell order. Current size: " << marketengine.get_orderbook().get_sells().size() << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_sells().begin()->second.front().priceTick, marketengine.get_market_price_ticks() + 2)
        << "The sell order price tick should be 1002 after submitting a sell order. Current price tick: " << marketengine.get_orderbook().get_sells().begin()->second.front().priceTick << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_sells().begin()->second.front().quantity, 100)
        << "The sell order quantity should be 100 after submitting a sell order. Current quantity: " << marketengine.get_orderbook().get_sells().begin()->second.front().quantity << "." << std::endl;
}

/**
    ============================================================
    TEST 4: StrategyPlacesQuotesOnInitialMarketUpdate
    ============================================================
    PURPOSE: Verify that after the first call to update(), the Strategy's on_market_update() method is triggered and places its initial buy and sell orders into the order book.
    ============================================================
*/
TEST(MarketEngineTest, StrategyPlacesQuotesOnInitialMarketUpdate) {
    MarketEngine marketengine(100, 2, 1000, 3, 0, 1000, 2, 1.0, 0.3);
    marketengine.update(1000);
    marketengine.execute_events_until(3000);
    EXPECT_EQ(marketengine.get_orderbook().get_buys().size(), 1)
        << "There should be one buy order in the orderbook after the first update. Current size: " << marketengine.get_orderbook().get_buys().size() << "." << std::endl;
    EXPECT_NE(marketengine.get_strategy().get_active_buy_order_id(), -1)
        << "Active buy order id should not be -1 after the first update. Current active buy order id: " << marketengine.get_strategy().get_active_buy_order_id() << "." << std::endl;
    EXPECT_EQ(marketengine.get_orderbook().get_sells().size(), 1)
        << "There should be one sell order in the orderbook after the first update. Current size: " << marketengine.get_orderbook().get_sells().size() << "." << std::endl;
    EXPECT_NE(marketengine.get_strategy().get_active_sell_order_id(), -1)
        << "Active sell order id should not be -1 after the first update. Current active sell order id: " << marketengine.get_strategy().get_active_sell_order_id() << "." << std::endl;
}

/**
============================================================
TEST 5: MarketPriceAndSpreadStableWithZeroVolatility
============================================================
PURPOSE: A corrected baseline. When volatility is 0, verify that calling update() multiple times does NOT change the market_price_ticks or the spread, as their calculations directly depend on a volatility of 0.
============================================================
*/
TEST(MarketEngineTest, MarketPriceAndSpreadStableWithZeroVolatility) {
    MarketEngine marketengine(100, 2, 1000, 3, 0, 1000, 2, 0.0, 0.3);
    marketengine.update(1000);
    marketengine.execute_events_until(3000);
    EXPECT_EQ(marketengine.get_market_price_ticks(), 1000)
        << "Market price should not have changed. Current market price: " << marketengine.get_market_price_ticks() << "." << std::endl;
    EXPECT_EQ(marketengine.get_spread(), 2)
        << "Spread should not have changed. Current spread: " << marketengine.get_spread() << "." << std::endl;
}
