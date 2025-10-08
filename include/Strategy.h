#pragma once

class Strategy {
    private:
        long long best_bid_ticks;
        long long best_ask_ticks;
        long long mid_price_ticks;
        long long current_market_price_ticks;
        long long spread_ticks;

        long long quote_size;
        long long tick_offset_from_mid;
        long long max_inventory;
        long long cancel_threshold_ticks;
        long long cooldown_between_requotes;

        long long active_buy_order_id;
        long long active_sell_order_id

    public:


};