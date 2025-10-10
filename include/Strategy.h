#pragma once

#include "Metrics.h"
#include "Order.h"
#include "OrderBook.h"

class Strategy {
    private:
        Metrics metrics;
        OrderBook order_book;
        
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
        long long active_sell_order_id;
        long long last_mid_price_ticks;
        long long last_quote_time_us;

        enum class State {
            WAITING_TO_BUY,
            WAITING_TO_SELL,
            BALANCED
        };
        State state;
    public:
        // Getters
        long long getBestBidTicks() const { return order_book.get_buys().rbegin()->first; }
        long long getBestAskTicks() const { return order_book.get_sells().begin()->first; }
        long long getMidPriceTicks() const { return (getBestBidTicks() + getBestAskTicks()) / 2; }
        long long getCurrentMarketPriceTicks() const { return metrics.last_mark_price_ticks; }
        long long getSpreadTicks() const { return getBestBidTicks() - getBestAskTicks(); }
        
        long long getQuoteSize() const { return quote_size; }
        long long getTickOffsetFromMid() const { return tick_offset_from_mid; }
        long long getMaxInventory() const { return max_inventory; }
        long long getCancelThresholdTicks() const { return cancel_threshold_ticks; }
        long long getCooldownBetweenRequotes() const { return cooldown_between_requotes; }
        
        long long getActiveBuyOrderId() const { return active_buy_order_id; }
        long long getActiveSellOrderId() const { return active_sell_order_id; }
        long long getLastMidPriceTicks() const { return last_mid_price_ticks; }
        long long getLastQuoteTimeUs() const { return last_quote_time_us; }
        
        State getState() const { return state; }
        
        // Setters
        void setQuoteSize(long long value) { 
            if (value > 0) {
                quote_size = value; 
            }
        }
        void setTickOffsetFromMid(long long value) { tick_offset_from_mid = value; }
        void setMaxInventory(long long value) { 
            if (value > 0) {
                max_inventory = value; 
            }
        }
        void setCancelThresholdTicks(long long value) { cancel_threshold_ticks = value; }
        void setCooldownBetweenRequotes(long long value) { 
            if (value > 0) {
                cooldown_between_requotes = value; 
            }
        }
        
        void setActiveBuyOrderId(long long value) { active_buy_order_id = value; }
        void setActiveSellOrderId(long long value) { active_sell_order_id = value; }
        void setLastMidPriceTicks(long long value) { 
            if (value > 0) {
                last_mid_price_ticks = value; 
            }
        }
        void setLastQuoteTimeUs(long long value) { 
            if (value > last_quote_time_us) {
                last_quote_time_us = value; 
            }       
        }
        
        void setState(State value) { state = value; }
};