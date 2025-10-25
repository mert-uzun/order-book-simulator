#pragma once

#include "OrderBook.h"
#include "Trade.h"
#include "Strategy.h"
#include <sys/stat.h>
#include <vector>

class MarketEngine {
    private:
        OrderBook orderbook;
        Strategy strategy;

        static long long env_order_id;
        static const double tick_size;

        std::random_device rng;
        std::mt19937 rand_engine;

        long long market_price_ticks;
        long long spread;
        double volatility;
        double fill_probability;
    public:
        MarketEngine(long long strategy_quote_size = 1, long long strategy_tick_offset= 1, long long strategy_max_inv = 10, long long strategy_cancel_threshold = 1, long long strategy_cooldown_between_requotes = 1, long long starting_mid_price = 10000, long long start_spread = 2, double start_vol = 1.0, double start_fill_prob = 0.3);
        
        void update(long long timestamp_us);

        void simulate_background_dynamics();
        void check_and_trigger_fills(long long timestamp_us);
        void execute_events_until(long long timestamp);

        // Getters
        OrderBook& get_orderbook() {
            return orderbook;
        }

        Strategy& get_strategy() {
            return strategy;
        }
};