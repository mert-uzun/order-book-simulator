#include "MarketEngine.h"
#include "Strategy.h"

class SimulationEngine {
    private:
        MarketEngine market_engine;

        long long starting_timestamp_us;
        long long current_timestamp_us;
        long long ending_timestamp_us;
        long long step_us;

    public:
        SimulationEngine(long long starting_timestamp_us, long long ending_timestamp_us, long long step_us, long long strategy_quote_size, long long strategy_tick_offset, long long strategy_max_inv, long long strategy_cancel_threshold, long long strategy_cooldown_between_requotes, long long starting_mid_price, long long start_spread, double start_vol, double start_fill_prob);
        void run();

        void finalize();

        long long get_starting_timestamp_us() { return starting_timestamp_us; }
        long long get_current_timestamp_us() { return current_timestamp_us; }
        long long get_ending_timestamp_us() { return ending_timestamp_us; }
        long long get_step_us() {return step_us; }
};