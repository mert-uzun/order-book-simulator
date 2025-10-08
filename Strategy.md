# Ping-Pong Market Maker Strategy – Design Document

## Overview

The **Ping-Pong Market Maker Strategy** is a simple but realistic algorithm used to test a limit-order-book simulator.  
It continuously places buy and sell quotes on both sides of the market to capture small profits from the bid-ask spread.  

When one side fills, it immediately places the opposite order to “bounce” back toward a flat (zero) position — hence the term **ping-pong**.  

This strategy exercises every part of the trading system:
- Order management and cancellation
- Trade execution
- PnL tracking via the `Metrics` module
- Risk controls (inventory, cooldown)
- Optional latency effects

---

## Conceptual Flow

The strategy runs as a continuous loop:

1) Observe the market (best bid / best ask)

2) Cancel or adjust outdated orders

3) Place new buy/sell quotes around the mid-price

4) Wait for a fill

5) When filled, immediately place the opposite order (ping-pong)

6) Update Metrics and repeat


Example in plain language:

> If a buy fills, immediately sell slightly higher.  
> If a sell fills, immediately buy slightly lower.  
> Keep oscillating around the mid-price and earning the spread.

---

## Example Walkthrough

Let’s assume:
- Tick size = \$0.01
- Quote size = 10 units
- Best bid = 100.00
- Best ask = 100.02
- Mid-price = 100.01
- Offset from mid = 1 tick (0.01)

### Step-by-Step

| Time | Event | Explanation |
|------|--------|-------------|
| t=0 | Market is 100.00 / 100.02 | The spread is 2 ticks. |
| t=0 | Strategy places: <br>• Buy 10 @ 100.00<br>• Sell 10 @ 100.02 | Quoting both sides. |
| t=5ms | **Buy fills** at 100.00 | Strategy now holds +10 inventory (bought 10). |
| t=5ms | Immediately places **Sell 10 @ 100.01** | The “pong” leg — exits one tick above the fill. |
| t=7ms | **Sell fills** at 100.01 | Strategy is back to 0 inventory. Profit = (100.01 − 100.00) × 10 = \$0.10. |
| t=7ms | Places new Buy/Sell pair again | Continues the loop. |

Over time, this creates a stream of small, consistent trades that each capture one or two ticks of profit per round trip.

---

## Class Responsibilities

### 1. Market Observation
The strategy must maintain a live view of the order book:
- `best_bid_ticks`
- `best_ask_ticks`
- `mid_price_ticks` = (bid + ask) / 2
- `spread_ticks` = (ask − bid)

This information comes from `on_book_update()` events.

**Purpose:**
Allows intelligent quoting — the strategy must know where to place orders relative to the current market.

---

### 2. Configuration Parameters

These control how aggressively the strategy trades.

| Parameter | Description | Example |
|------------|-------------|----------|
| `quote_size` | Quantity per order | 10 |
| `tick_offset_from_mid` | How far from the mid-price to quote | 1 tick |
| `max_inventory` | Maximum allowed position | 50 units |
| `cancel_threshold_ticks` | How far the market can move before old quotes are cancelled | 2 |
| `cooldown_time_us` | Minimum time between re-quotes | 100,000 µs (0.1s) |

These settings define the “personality” of the market maker:
- Small offsets = more aggressive, more fills, more risk.
- Large offsets = safer but fewer fills.

---

### 3. State Tracking

| Member | Purpose |
|---------|----------|
| `active_buy_order_id` | ID of the currently working buy order. |
| `active_sell_order_id` | ID of the currently working sell order. |
| `last_mid_price_ticks` | Last mid-price used for quoting (to detect market movement). |
| `last_quote_time_us` | Last time a quote was updated (to enforce cooldown). |
| `state` (optional) | Tracks phase: waiting_to_buy / waiting_to_sell / balanced. |

The strategy can also query:
- `Metrics.get_position()` to know current inventory.
- `Metrics.order_cache` to see live orders and their remaining size.

---

### 4. Decision Logic (Ping-Pong Core)

#### Step 1: React to Market Updates
When a new best bid/ask arrives:
1. Compute `mid = (bid + ask)/2`.
2. If `mid` moved by more than `cancel_threshold_ticks`, cancel old quotes.
3. Respect `cooldown_time_us` before re-quoting.

#### Step 2: Risk Check
- If current inventory ≥ `max_inventory`, skip placing new **buys**.
- If current inventory ≤ −`max_inventory`, skip placing new **sells**.

#### Step 3: Place New Quotes
If no active buy:
> Place a buy at `(mid − tick_offset_from_mid)` for `quote_size`.

If no active sell:
> Place a sell at `(mid + tick_offset_from_mid)` for `quote_size`.

Record the order IDs and update `last_quote_time_us`.

#### Step 4: Handle Fills
When a fill occurs:
- If it was a **buy fill** → inventory increases.  
  → Immediately place a **sell** one tick above to exit profitably.
- If it was a **sell fill** → inventory decreases.  
  → Immediately place a **buy** one tick below to rebalance.

This “ping-pong” ensures the strategy oscillates around zero position while repeatedly capturing the spread.

#### Step 5: Evaluate
Periodically check:
- Profit/loss (via `Metrics`)
- Fill ratio
- Max drawdown  
If losses or inventory grow too large, widen quotes or pause trading.

---

### 5. External References

| Reference | Purpose |
|------------|----------|
| `OrderBook*` | Send/cancel orders and access best bid/ask prices. |
| `Metrics*` | Read position, PnL, and order cache. |
| `LatencyQueue*` (optional) | Delay messages once latency simulation is added. |

The Strategy **does not** execute trades directly — it requests them from the `OrderBook`.

---

## Visualization of the Control Loop

┌───────────────────────────┐
│ Market Data │
│ (best bid / best ask) │
└──────────────┬────────────┘
│
▼
┌────────────┐
│ Strategy │
│ Ping-Pong │
└─────┬──────┘
│ orders / cancels
▼
┌────────────┐
│ OrderBook │
│ (simulator)│
└─────┬──────┘
│ trades executed
▼
┌────────────┐
│ Metrics │
│ (PnL, risk)│
└────────────┘


---

## Why Ping-Pong Matters

Even though simple, this strategy stresses every component of the system:
- Constant order creation, cancellation, and matching.
- Frequent inventory swings (testing Metrics accuracy).
- Sensitivity to latency and tick size.
- Realistic trade flow patterns for backtesting.

When the simulator handles this smoothly, the underlying architecture is proven robust.

---

## Summary of Required Members

| Category | Member | Purpose |
|-----------|---------|----------|
| **Market Data** | `best_bid_ticks`, `best_ask_ticks`, `mid_price_ticks`, `spread_ticks` | Live market snapshot. |
| **Configuration** | `quote_size`, `tick_offset_from_mid`, `max_inventory`, `cancel_threshold_ticks`, `cooldown_time_us` | Defines quoting style. |
| **State Tracking** | `active_buy_order_id`, `active_sell_order_id`, `last_mid_price_ticks`, `last_quote_time_us`, `state` | Remembers what is currently active. |
| **External Links** | `OrderBook*`, `Metrics*` | Interfaces to the rest of the system. |

---

## Key Takeaways

- The Ping-Pong strategy **always maintains two-sided liquidity**.
- Every time one side fills, the opposite side is placed immediately.
- Position is kept near zero; profit comes from repeatedly capturing small spreads.
- Metrics tracks PnL, fill ratio, and inventory to evaluate performance.
- Later, the **Latency Queue** will delay order placement and fills to simulate real-world lag.

When this strategy runs smoothly and Metrics show stable positive results, the simulator is fully validated.

