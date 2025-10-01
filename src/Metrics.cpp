#include "../include/Metrics.h"

void Metrics::set_config(double tick_size, long long maker_rebate_per_share_ticks, long long taker_fee_per_share_ticks, MarkingMethod marking_method) {
    config.tick_size = tick_size;
    config.maker_rebate_per_share_ticks = maker_rebate_per_share_ticks;
    config.taker_fee_per_share_ticks = taker_fee_per_share_ticks;
    config.marking_method = marking_method;
}

void Metrics::reset() {
    config.tick_size = 0;
    config.maker_rebate_per_share_ticks = 0;
    config.taker_fee_per_share_ticks = 0;
    config.marking_method = MarkingMethod::MID;

    fees_ticks = 0;
    position = 0;
    average_entry_price_ticks = 0;
    realized_pnl_ticks = 0;
    unrealized_pnl_ticks = 0;
    total_pnl_ticks = 0;

    gross_traded_qty = 0;
    resting_attempted_qty = 0;
    resting_filled_qty = 0;
    resting_cancelled_qty = 0;

    equity_value_peak_ticks = 0;
    max_dropdown_ticks = 0;
    returns_series.clear();

    current_best_ask_price_ticks = 0;
    current_best_bid_price_ticks = 0;
    last_trade_price_ticks = 0;
    last_mark_price_ticks = 0;

    order_cache.clear();
}

void Metrics::finalize() {

}

void Metrics::on_order_placed(long long order_id, Side side, long long arrival_price_ticks, long long arrival_timestamp_us, int intended_quantity, bool is_instant) {

}

void Metrics::on_order_cancelled(long long order_id, int remaining_qty, long long delete_timestamp_us) {

}

void Metrics::on_fill(long long order_id, Side side, long long fill_price_ticks, long long fill_timestamp_us, int filled_quantity, bool was_instant) {

}

void Metrics::on_market_price_update() {

}


int Metrics::get_position() {
    return position;
}

long long Metrics::get_avg_entry_price_ticks() {
    return average_entry_price_ticks;
}

long long Metrics::get_realized_pnl_ticks() {
    return realized_pnl_ticks;
}

long long Metrics::get_unrealized_pnl_ticks() {
    return unrealized_pnl_ticks;
} 

long long Metrics::get_total_pnl_ticks() {
    return total_pnl_ticks;
}

long long Metrics::get_gross_traded_qty() {
    return gross_traded_qty;
}

double Metrics::get_fill_ratio() {
    if (resting_attempted_qty == 0) {
        return 0;
    }

    return double(resting_filled_qty) / resting_attempted_qty;
}

long long Metrics::get_max_drawdown_ticks() {
    return max_dropdown_ticks;
}
