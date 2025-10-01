#pragma once

#include <unordered_map>
#include <vector>

class Metrics {
        public:
        enum class MarkingMethod {
            MID,
            LAST,
            BID_ASK_MID
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

        long long last_mark_price_ticks;

        struct OrderCacheData {
            long long arrival_mark_price_ticks;
            int intended_quantity;
            int remaining_qty;
            int is_ioc;
        };

        std::unordered_map<long long, OrderCacheData> order_cache;

        void set_config(double tick_size, MarkingMethod marking_method, long long fee_per_unit_for_ioc, long long fees);
        void reset();
        void finalize();
        void on_order_placed(long long order_id, Side side);

};