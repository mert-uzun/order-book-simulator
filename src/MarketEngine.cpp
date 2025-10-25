#include "../include/MarketEngine.h"

MarketEngine::MarketEngine(long long strategy_quote_size, long long strategy_tick_offset, long long strategy_max_inv, long long strategy_cancel_threshold, long long strategy_cooldown_between_requotes) : orderbook(), strategy(orderbook, strategy_quote_size, strategy_tick_offset, strategy_max_inv, strategy_cancel_threshold, strategy_cooldown_between_requotes) {}
        
void MarketEngine::update(long long timestamp_us) {
    simulate_background_dynamics();

    check_and_trigger_fills();

    strategy.on_market_update(timestamp_us);
}

void MarketEngine::simulate_background_dynamics() {

}

void MarketEngine::check_and_trigger_fills() {

}

