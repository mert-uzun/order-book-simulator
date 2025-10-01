#pragma once

#include <unordered_map>
#include <vector>

class Metrics {
        public:
        enum class MarkingMethod {
            MID,
            LAST
        };

        enum class Side {
            BUYS,
            SELLS
        };

        struct Config {
            double tick_size;
            long long maker_rebate_per_share_ticks;
            long long taker_fee_per_share_ticks;
            MarkingMethod marking_method;
        };

        long long fees_ticks;
        
        int position;
        long long average_entry_price_ticks;
        long long realized_pnl_ticks;
        long long unrealized_pnl_ticks;
        long long total_pnl_ticks;

        long gross_traded_qty;
        long resting_attempted_qty;
        long resting_filled_qty;
        long resting_cancelled_qty;

        long long equity_peak_ticks;
        long long max_dropdown_ticks;
        std::vector<double> returns_series;

        long long current_best_bid_price_ticks;
        long long current_best_ask_price_ticks;
        long long last_trade_price_ticks;
        long long last_mark_price_ticks; // This is the price where you last calculated PnL

        struct OrderCacheData {
            Side side;
            long long arrival_mark_price_ticks;
            long long arrival_timestamp_us;
            int intended_quantity;
            int remaining_qty;
            int is_ioc;    
        };

        std::unordered_map<long long, OrderCacheData> order_cache;

        void set_config(double tick_size, long long maker_rebate_per_share_ticks, long long taker_fee_per_share_ticks, MarkingMethod marking_method);
        void reset();
        void finalize();
        void on_order_placed(long long order_id, Side side, long long arrival_price_ticks, long long arrival_timestamp_us, int intended_quantity, bool is_instant);
        void on_order_cancelled(long long order_id, int remaining_qty, long long delete_timestamp_us);
        void on_fill(long long order_id, Side side, long long fill_price_ticks, long long fill_timestamp_us, int filled_quantity, bool was_instant);
        void on_market_price_update(MarkingMethod marking_method);

        int get_position();
        long long get_avg_entry_price_ticks();
        long long get_realized_pnl_ticks();
        long long get_unrealized_pnl_ticks(); 
        long long get_total_pnl_ticks();
        long long get_gross_traded_qty();
        long long get_fill_ratio();
        long long get_max_drawdown_ticks();
};