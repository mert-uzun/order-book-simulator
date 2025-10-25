#pragma once

#include "OrderBook.h"
#include "Trade.h"
#include "Strategy.h"

class MarketEngine {
    private:
        OrderBook orderbook;
        Strategy strategy;

        long long market_price_ticks;
        long long spread;
        double volatility;
        double fill_probability;

    public:
        MarketEngine(long long strategy_quote_size = 1, long long strategy_tick_offset= 1, long long strategy_max_inv = 10, long long strategy_cancel_threshold = 1, long long strategy_cooldown_between_requotes = 1);
        
        void update(long long);

        void simulate_background_dynamics();
        void check_and_trigger_fills();

        // Getters
        OrderBook& get_orderbook() {
            return orderbook;
        }

        Strategy& get_strategy() {
            return strategy;
        }
};