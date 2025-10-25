#include "../include/SimulationEngine.h"

SimulationEngine::SimulationEngine(long long starting_timestamp_us, long long ending_timestamp_us, long long step_us, long long strategy_quote_size, long long strategy_tick_offset, long long strategy_max_inv, long long strategy_cancel_threshold, long long strategy_cooldown_between_requotes, long long starting_mid_price, long long start_spread, double start_vol, double start_fill_prob)
                                    : starting_timestamp_us(starting_timestamp_us), current_timestamp_us(starting_timestamp_us), ending_timestamp_us(ending_timestamp_us), step_us(step_us), market_engine(strategy_quote_size, strategy_tick_offset, strategy_max_inv, strategy_cancel_threshold, strategy_cooldown_between_requotes, starting_mid_price, start_spread, start_vol, start_fill_prob) {}

void SimulationEngine::run() {
    
}

void SimulationEngine::finalize() {

}