#include "../include/Strategy.h"

Strategy::Strategy(long long quote_size, long long tick_offset, long long max_inv, long long cancel_threshold, long long cooldown_between_requotes) 
                        : metrics(), order_book(), best_bid_ticks(0), best_ask_ticks(0), mid_price_ticks(0), current_market_price_ticks(0), spread_ticks(0), current_inventory(0),
                        quote_size(quote_size), tick_offset_from_mid(tick_offset), max_inventory(max_inv), cancel_threshold_ticks(cancel_threshold), cooldown_between_requotes(cooldown_between_requotes), 
                        active_buy_order_id(-1), active_sell_order_id(-1), last_mid_price_ticks(0), last_quote_time_us(0) {};

void Strategy::observe_the_market() {
    best_bid_ticks = get_best_bid_ticks();
    best_ask_ticks = get_best_ask_ticks();
    mid_price_ticks = get_mid_price_ticks();
    spread_ticks = get_spread_ticks();
}

void Strategy::check_cancel() {
    observe_the_market();
    
    if (mid_price_ticks - last_mid_price_ticks > cancel_threshold_ticks) {
        order_book.cancel_order(active_buy_order_id);
        order_book.cancel_order(active_sell_order_id);
        metrics.on_order_cancelled(active_buy_order_id, long long delete_timestamp_us);
        metrics.on_order_cancelled(active_sell_order_id, long long delete_timestamp_us);
    }
}

void Strategy::update_last_used_mark_price() {
    last_mid_price_ticks = mid_price_ticks;
}

long long Strategy::place_buy() {
    long long order_id = order_book.add_limit_order(true, current_market_price_ticks - tick_offset_from_mid, quote_size, long long timestamp);
    metrics.on_order_placed(order_id, Metrics::Side::BUYS, current_market_price_ticks - tick_offset_from_mid, long long arrival_timestamp_us, quote_size, false);
    return order_id;
}

long long Strategy::place_ask() {
    long long order_id = order_book.add_limit_order(false, current_market_price_ticks + tick_offset_from_mid, quote_size, long long timestamp);
    metrics.on_order_placed(order_id, Metrics::Side::SELLS, current_market_price_ticks + tick_offset_from_mid, long long arrival_timestamp_us, quote_size, false);
    return order_id;
}