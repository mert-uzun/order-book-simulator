#include "../include/Strategy.h"
#include "../include/Trade.h"
#include <cstdlib>

Strategy::Strategy(long long quote_size, long long tick_offset, long long max_inv, long long cancel_threshold, long long cooldown_between_requotes) 
                        : metrics(), order_book(), latency_queue(), best_bid_ticks(0), best_ask_ticks(0), mid_price_ticks(0), current_market_price_ticks(0), spread_ticks(0), current_inventory(0),
                        quote_size(quote_size), tick_offset_from_mid(tick_offset), max_inventory(max_inv), cancel_threshold_ticks(cancel_threshold), cooldown_between_requotes(cooldown_between_requotes), 
                        active_buy_order_id(-1), active_sell_order_id(-1), last_mid_price_ticks(0), last_quote_time_us(0) {};

void Strategy::observe_the_market(long long timestamp_us) {
    latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::MARKET_UPDATE, [&]() {
        best_bid_ticks = get_best_bid_ticks();
        best_ask_ticks = get_best_ask_ticks();
        mid_price_ticks = get_mid_price_ticks();
        spread_ticks = get_spread_ticks();
    
        if (metrics.config.marking_method == Metrics::MarkingMethod::MID) {
            current_market_price_ticks = mid_price_ticks;
        }
        else {
            current_market_price_ticks = metrics.last_trade_price_ticks;
        }
    });
}

void Strategy::cancel_mechanism(long long timestamp_us) {
    observe_the_market(timestamp_us);

    latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::CANCEL, [&]() {
        if (abs(mid_price_ticks - last_mid_price_ticks) > cancel_threshold_ticks) {
            order_book.cancel_order(active_buy_order_id);
            order_book.cancel_order(active_sell_order_id);
            metrics.on_order_cancelled(active_buy_order_id, timestamp_us);
            metrics.on_order_cancelled(active_sell_order_id, timestamp_us);
    
        }
    });
}

void Strategy::update_last_used_mark_price() {
    last_mid_price_ticks = mid_price_ticks;
}

long long Strategy::place_ping_buy(long long timestamp_us) {
    if (timestamp_us - last_quote_time_us > cooldown_between_requotes && current_inventory + quote_size <= max_inventory) {
        long long order_id = order_book.add_limit_order(true, current_market_price_ticks - tick_offset_from_mid, quote_size, timestamp_us);
        metrics.on_order_placed(order_id, Metrics::Side::BUYS, current_market_price_ticks - tick_offset_from_mid, timestamp_us, quote_size, false);
        last_quote_time_us = timestamp_us;
        return order_id;
    }
    else {
        return -1;
    }
}

long long Strategy::place_ping_ask(long long timestamp_us) {
    if (timestamp_us - last_quote_time_us > cooldown_between_requotes && current_inventory - quote_size >= -max_inventory) {

        long long order_id = order_book.add_limit_order(false, current_market_price_ticks + tick_offset_from_mid, quote_size, timestamp_us);
        metrics.on_order_placed(order_id, Metrics::Side::SELLS, current_market_price_ticks + tick_offset_from_mid, timestamp_us, quote_size, false);
        last_quote_time_us = timestamp_us;
        return order_id;
    }
    else {
        return -1;
    }
}

bool Strategy::is_bid_filled(long long order_id) {
    return metrics.order_cache.find(order_id) == metrics.order_cache.end();
}

long long Strategy::pong_on_bid_filled(long long timestamp_us) {
    

    return active_buy_order_id;
}

bool Strategy::is_ask_filled(long long order_id) {
    return metrics.order_cache.find(order_id) == metrics.order_cache.end();
}

long long Strategy::pong_on_ask_filled(long long timestamp_us) {
    

    return active_sell_order_id;
}

void Strategy::on_market_update(long long timestamp) {
    observe_the_market();
    cancel_mechanism(timestamp);

    if (is_bid_filled(active_buy_order_id)) {
        active_buy_order_id = place_ping_buy(timestamp);
    }

    if (is_ask_filled(active_sell_order_id)) {
        active_sell_order_id = place_ping_ask(timestamp);
    }
    /*
        Marketi gör, değerleri güncelle
        cancellayacak kadar oynamış mı bak, eğer oynadıysa cancel et
        cancel ettiysen yerine yenisini koy mutlaka

        PING: eğer active buy ya da active sell yoksa koy
    */
}

void Strategy::on_fill(const Trade& trade) {
    if (trade.buyOrderId == active_buy_order_id) {
        int remaining_qty = metrics.order_cache.find(active_buy_order_id)->second.remaining_qty - trade.quantity;
        metrics.on_fill(active_buy_order_id, trade.priceTick, trade.timestampUs, trade.quantity, trade.was_instant);

        if (remaining_qty == 0) {
            active_buy_order_id = -1;
        }

        long long pong_order_id = order_book.add_limit_order(false, trade.priceTick + 1, trade.quantity,  trade.timestampUs);
        metrics.on_order_placed(pong_order_id, Metrics::Side::SELLS, trade.priceTick + 1, trade.timestampUs, trade.quantity, false);

        current_inventory += trade.quantity;
    }
    else if (trade.sellOrderId == active_sell_order_id) {
        int remaining_qty = metrics.order_cache.find(active_sell_order_id)->second.remaining_qty - trade.quantity;
        metrics.on_fill(active_sell_order_id, trade.priceTick, trade.timestampUs, trade.quantity, trade.was_instant);

        if (remaining_qty == 0) {
            active_sell_order_id = -1;
        }

        long long pong_order_id = order_book.add_limit_order(true, trade.priceTick - 1, trade.quantity, trade.timestampUs);
        metrics.on_order_placed(pong_order_id, Metrics::Side::BUYS, trade.priceTick - 1, trade.timestampUs, trade.quantity, false);

        current_inventory -= trade.quantity;
    }

    /*
        PONG: eğer buy alınırsa aynısının karşısına bi sell yerleştir, eğer sell alınırsa karşısına buy yerleştir
    */
}