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
        MarketEngine();
        
        void update();

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