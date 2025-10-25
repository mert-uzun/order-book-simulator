#include "../include/MarketEngine.h"
#include <algorithm>
#include <cmath>
#include <random>

const double MarketEngine::tick_size = 0.001;

MarketEngine::MarketEngine(long long strategy_quote_size, long long strategy_tick_offset, long long strategy_max_inv, long long strategy_cancel_threshold, long long strategy_cooldown_between_requotes, long long starting_mid_price, long long start_spread, double start_vol, double start_fill_prob) : orderbook(), 
                            strategy(orderbook, strategy_quote_size, strategy_tick_offset, strategy_max_inv, strategy_cancel_threshold, strategy_cooldown_between_requotes), market_price_ticks(starting_mid_price), spread(start_spread), volatility(start_vol), fill_probability(start_fill_prob) {}

void MarketEngine::update(long long timestamp_us) {
    simulate_background_dynamics();

    check_and_trigger_fills();

    strategy.on_market_update(timestamp_us);
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

void MarketEngine::check_and_trigger_fills() {

}

