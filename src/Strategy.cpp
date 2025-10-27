#include "../include/Strategy.h"
#include "../include/Trade.h"
#include <cstdlib>
#include <stdexcept>
#include <utility>

Strategy::Strategy(OrderBook& orderbook, long long quote_size, long long tick_offset, long long max_inv, long long cancel_threshold, long long cooldown_between_requotes) 
                        : metrics(), order_book(orderbook), buy_pongs(), sell_pongs(), latency_queue(), best_bid_ticks(0), best_ask_ticks(0), mid_price_ticks(0), current_market_price_ticks(0), spread_ticks(0),
                        quote_size(quote_size), tick_offset_from_mid(tick_offset), max_inventory(max_inv), cancel_threshold_ticks(cancel_threshold), cooldown_between_requotes(cooldown_between_requotes), 
                        active_buy_order_id(-1), active_sell_order_id(-1), last_pinged_mid_price_ticks(0), last_quote_time_us(0) {};

void Strategy::observe_the_market(long long timestamp_us, long long market_price) {
    latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::MARKET_UPDATE, [this, market_price](long long exec_time) {
        best_bid_ticks = get_best_bid_ticks();
        best_ask_ticks = get_best_ask_ticks();
        mid_price_ticks = get_mid_price_ticks();
        spread_ticks = get_spread_ticks();

        // Since the system now uses a dynamic market generating engine, these are commented. Uncomment when system uses a full-order-book as market.
        // if (metrics.config.marking_method == Metrics::MarkingMethod::MID) {
        //     current_market_price_ticks = mid_price_ticks;
        // }
        // else {
        //     current_market_price_ticks = metrics.last_trade_price_ticks;
        // }

        current_market_price_ticks = market_price;
    });
}

void Strategy::cancel_mechanism(long long timestamp_us) {
    if (abs(mid_price_ticks - last_pinged_mid_price_ticks) > cancel_threshold_ticks) {
        latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::CANCEL, [this, timestamp_us](long long exec_time) {
            if (active_buy_order_id != -1) {
                order_book.cancel_order(active_buy_order_id);
                metrics.on_order_cancelled(active_buy_order_id, exec_time);
                active_buy_order_id = -1;
            }
            if (active_sell_order_id != -1) {
                order_book.cancel_order(active_sell_order_id);
                metrics.on_order_cancelled(active_sell_order_id, exec_time);
                active_sell_order_id = -1;
            }
        });
    }    
}

void Strategy::update_last_used_mark_price() {
    last_pinged_mid_price_ticks = mid_price_ticks;
}

void Strategy::place_ping_buy(long long timestamp_us) {
    if (timestamp_us - last_quote_time_us > cooldown_between_requotes && metrics.position + quote_size <= max_inventory) {
        latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::ORDER_SEND, [this, timestamp_us](long long exec_time) {
            long long order_id = order_book.add_limit_order(true, current_market_price_ticks - tick_offset_from_mid, quote_size, exec_time);
            metrics.on_order_placed(order_id, Metrics::Side::BUYS, current_market_price_ticks - tick_offset_from_mid, exec_time, quote_size, false);
            last_quote_time_us = exec_time;
            active_buy_order_id = order_id;

            update_last_used_mark_price();
        });
    }
}

void Strategy::place_ping_ask(long long timestamp_us) {
    if (timestamp_us - last_quote_time_us > cooldown_between_requotes && metrics.position - quote_size >= -max_inventory) {
        latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::ORDER_SEND, [this, timestamp_us](long long exec_time) {
            long long order_id = order_book.add_limit_order(false, current_market_price_ticks + tick_offset_from_mid, quote_size, exec_time);
            metrics.on_order_placed(order_id, Metrics::Side::SELLS, current_market_price_ticks + tick_offset_from_mid, exec_time, quote_size, false);
            last_quote_time_us = exec_time;
            active_sell_order_id = order_id;

            update_last_used_mark_price();
        });
    }
}

bool Strategy::is_bid_ping_filled(long long order_id) {
    return metrics.order_cache.find(order_id) == metrics.order_cache.end();
}

bool Strategy::is_ask_ping_filled(long long order_id) {
    return metrics.order_cache.find(order_id) == metrics.order_cache.end();
}

void Strategy::on_market_update(long long timestamp, long long market_price) {
    observe_the_market(timestamp, market_price);
    cancel_mechanism(timestamp);

    if (is_bid_ping_filled(active_buy_order_id)) {
        place_ping_buy(timestamp);
    }

    if (is_ask_ping_filled(active_sell_order_id)) {
        place_ping_ask(timestamp);
    }

    check_and_fill_pongs(market_price, timestamp);
    
    /*
        Observe the market, update the values
        See if the market moves enough to cancel previous pings, cancel if yes
        Send new pings if you cancelled in the previous step
        
        PING: if there are no active buy or active sell
    */
}

void Strategy::on_fill(const Trade& trade) {
    if (trade.buyOrderId == active_buy_order_id) {
        latency_queue.schedule_event(trade.timestampUs, LatencyQueue::ActionType::ACKNOWLEDGE_FILL, [this, trade](long long ack_exec_time) {
            int remaining_qty = metrics.order_cache.find(trade.buyOrderId)->second.remaining_qty - trade.quantity;
            metrics.on_fill(trade.buyOrderId, trade.priceTick, ack_exec_time, trade.quantity, trade.was_instant);

            if (remaining_qty == 0 && active_buy_order_id == trade.buyOrderId) {
                active_buy_order_id = -1;
                order_book.cancel_order(trade.buyOrderId);
            }

            latency_queue.schedule_event(ack_exec_time, LatencyQueue::ActionType::ORDER_SEND, [this, trade](long long exec_time) {
                long long pong_order_id = order_book.add_limit_order(false, trade.priceTick + 1, trade.quantity,  exec_time);
                sell_pongs.emplace(trade.priceTick + 1, std::pair<long long, int>(pong_order_id, trade.quantity));
                metrics.on_order_placed(pong_order_id, Metrics::Side::SELLS, trade.priceTick + 1, exec_time, trade.quantity, false);
            });            
        });        
    }
    else if (trade.sellOrderId == active_sell_order_id) {
        latency_queue.schedule_event(trade.timestampUs, LatencyQueue::ActionType::ACKNOWLEDGE_FILL, [this, trade](long long ack_exec_time) {
            int remaining_qty = metrics.order_cache.find(trade.sellOrderId)->second.remaining_qty - trade.quantity;
            metrics.on_fill(trade.sellOrderId, trade.priceTick, ack_exec_time, trade.quantity, trade.was_instant);
    
            if (remaining_qty == 0 && active_sell_order_id == trade.sellOrderId) {
                active_sell_order_id = -1;
                order_book.cancel_order(trade.sellOrderId);
            }

            latency_queue.schedule_event(ack_exec_time, LatencyQueue::ActionType::ORDER_SEND, [this, trade](long long exec_time) {
                long long pong_order_id = order_book.add_limit_order(true, trade.priceTick - 1, trade.quantity, exec_time);
                buy_pongs.emplace(trade.priceTick - 1, std::pair<long long, int>(pong_order_id, trade.quantity));
                metrics.on_order_placed(pong_order_id, Metrics::Side::BUYS, trade.priceTick - 1, exec_time, trade.quantity, false);
            });
        });
    }

    /*
        PONG: place a sell at the opposite if bid ping is filled, vice versa
    */

    /*
        Problem: I needed to somehow pass the randomized latency into the lambda passed to schedule_event - solved, I passed the actual execution time to the lambdas, so they can use this information internally whenever they are executed.
    */
}

void Strategy::check_and_fill_pongs(long long market_price, long long timestamp_us) {
    // Check for pong bids, if market price is below them consider them filled.
    while (!buy_pongs.empty() && market_price <= buy_pongs.top().first) {
        PongOrderData highest_bid_pong_order_data = buy_pongs.top();
            
        latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::ACKNOWLEDGE_FILL, [this, highest_bid_pong_order_data](long long exec_time) {
            metrics.on_fill(highest_bid_pong_order_data.second.first, highest_bid_pong_order_data.first, exec_time, highest_bid_pong_order_data.second.second, false);
            order_book.cancel_order(highest_bid_pong_order_data.second.first);
        });
    
        buy_pongs.pop();
    }    

    while (!sell_pongs.empty() && market_price >= sell_pongs.top().first) {
        PongOrderData lowest_sell_pong_order_data = sell_pongs.top();
    
        latency_queue.schedule_event(timestamp_us, LatencyQueue::ActionType::ACKNOWLEDGE_FILL, [this, lowest_sell_pong_order_data](long long exec_time) {
            metrics.on_fill(lowest_sell_pong_order_data.second.first, lowest_sell_pong_order_data.first, exec_time, lowest_sell_pong_order_data.second.second, false);
            order_book.cancel_order(lowest_sell_pong_order_data.second.first);
        });

        sell_pongs.pop();
    }
}

Metrics::OrderCacheData Strategy::get_active_buy_order_data() {
    if (active_buy_order_id != -1) {
        return metrics.order_cache.find(active_buy_order_id)->second;
    }
    else {
        throw std::runtime_error("You cannot get data from metrics.order_cache when active buy order id is -1!");
    }
}

Metrics::OrderCacheData Strategy::get_active_sell_order_data() {
    if (active_sell_order_id != -1) {
        return metrics.order_cache.find(active_sell_order_id)->second;
    }
    else {
        throw std::runtime_error("You cannot get data from metrics.order_cache when active sell order id is -1!");
    }
}
