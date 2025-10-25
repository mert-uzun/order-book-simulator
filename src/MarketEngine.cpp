#include "../include/MarketEngine.h"
#include <algorithm>
#include <cmath>
#include <random>

long long MarketEngine::env_order_id = 1000000;
const double MarketEngine::tick_size = 0.001;

MarketEngine::MarketEngine(long long strategy_quote_size, long long strategy_tick_offset, long long strategy_max_inv, long long strategy_cancel_threshold, long long strategy_cooldown_between_requotes, long long starting_mid_price, long long start_spread, double start_vol, double start_fill_prob) : orderbook(), 
                            strategy(orderbook, strategy_quote_size, strategy_tick_offset, strategy_max_inv, strategy_cancel_threshold, strategy_cooldown_between_requotes), market_price_ticks(starting_mid_price), spread(start_spread), volatility(start_vol), fill_probability(start_fill_prob) {}

void MarketEngine::update(long long timestamp_us) {
    simulate_background_dynamics();

    check_and_trigger_fills(timestamp_us);

    strategy.on_market_update(timestamp_us);

    execute_events_until(timestamp_us);
}

void MarketEngine::simulate_background_dynamics() {
    std::random_device rng;
    std::mt19937 engine(rng);

    std::uniform_int_distribution<int> move(-1, 1);
    std::uniform_real_distribution<double> jump(0.0, 1.0);

    long long prev_price_ticks = market_price_ticks;

    market_price_ticks += move(engine) * volatility;

    // A rare jump in the market simulating news
    if (jump(engine) < 0.001) {
        market_price_ticks += move(engine) * (5 * volatility);
    }

    long long price_change = market_price_ticks - prev_price_ticks;

    spread = std::max<long long>(1, (long long)(2 + 0.5 * volatility)); 

    volatility = std::sqrt((0.1 * price_change * price_change) + 0.9 * volatility);

    double base_fill = 0.002;
    double k = 0.2;
    double alpha = 0.1;

    fill_probability = (1 - alpha) * fill_probability + alpha * (base_fill * std::exp(-k * volatility));
}

void MarketEngine::check_and_trigger_fills(long long timestamp_us) {
    std::random_device rnd;
    std::mt19937 rand_engine(rnd);
    std::uniform_real_distribution<double> probability(0, 1);
    std::uniform_int_distribution<int> filled_quantity(0, strategy.get_quote_size());

    double k = 0.5; // How fast fill probability decays based on distance from market price

    // -------------- BUY SIDE -------------- //
    if (strategy.get_active_buy_order_id() != -1) {
        Metrics::OrderCacheData order_data = strategy.get_active_buy_order_data();
        long long order_price = order_data.arrival_mark_price_ticks;
        long long dist = market_price_ticks - order_price;

        double fill_prob = fill_probability * std::exp(-k * dist);
        fill_prob = std::min<double>(1, fill_prob);

        std::uniform_int_distribution<long long> fill_price(order_price, market_price_ticks + spread / 2);

        if (probability(rand_engine) < fill_prob) {
            Trade trade(strategy.get_active_buy_order_id(), env_order_id++, fill_price(rand_engine), std::min<int>(order_data.remaining_qty, filled_quantity(rand_engine)), timestamp_us, false);
            strategy.on_fill(trade);
        }
    }

    // -------------- SELL SIDE -------------- //
    if (strategy.get_active_sell_order_id() != -1) {
        Metrics::OrderCacheData order_data = strategy.get_active_sell_order_data();
        long long order_price = order_data.arrival_mark_price_ticks;
        long long dist = order_price - market_price_ticks;

        double fill_prob = fill_probability * std::exp(-k * dist);
        fill_prob = std::min<double>(1, fill_prob);

        std::uniform_int_distribution<long long> fill_price(market_price_ticks - spread / 2, order_price);

        if (probability(rand_engine) < fill_prob) {
            Trade trade(env_order_id++, strategy.get_active_sell_order_id(), fill_price(rand_engine), std::min<int>(order_data.remaining_qty, filled_quantity(rand_engine)), timestamp_us, false);
            strategy.on_fill(trade);
        }
    }    
}

void MarketEngine::execute_events_until(long long timestamp) {
    strategy.execute_latency_queue(timestamp);
}