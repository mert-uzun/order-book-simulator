# Metrics: Purpose

Track **strategy performance** while trading in your simulated market. It consumes **fills** and **quotes**, and produces:

* Real-time **P&L** (realized + unrealized)
* **Inventory**/exposure and turnover
* **Execution quality** (slippage / implementation shortfall, fill ratio, maker vs taker)
* **Risk** (drawdown, Sharpe, volatility of returns)
* Clean **time series** for plotting and summaries for reports

Everything is computed in **ticks** internally for precision; convert to currency only for display.

---

# Configuration (immutable after init)

Store once; used in all calculations.

* `tick_size` (double): currency per tick (e.g., 0.01).
* `maker_rebate_per_share` (double, currency) and `taker_fee_per_share` (double, currency).

  * Precompute as ticks: `maker_rebate_ticks = maker_rebate_per_share / tick_size`, etc.
* `return_bucket_us` (int64): time bucket for returns (e.g., 1_000_000 µs = 1s).
* `mark_method` (enum): how to mark unrealized P&L (`MID`, `LAST`, `BID_ASK_MID`).
* `risk_limits` (struct, optional): inventory cap, max notional, etc. (for alerts, not enforcement).

**Methods**

* `set_config(cfg)` (once, before run)
* `get_config()` (read-only)

---

# Core state (updated during simulation)

## Position & P&L

* `position` (int64): signed; >0 long, <0 short.
* `avg_entry_price_ticks` (double): VWAP of your **open** position (resets on flips).
* `realized_pnl_ticks` (double)
* `unrealized_pnl_ticks` (double; recomputed on each mark)
* `total_pnl_ticks` (double) = realized + unrealized

## Cashflow & fees

* `maker_qty` (int64), `taker_qty` (int64)
* `fees_ticks` (double): negative for taker fees, positive for maker rebates (rebates increase realized P&L)
* `gross_traded_qty` (int64): sum of absolute filled quantity

## Execution quality

* `sum_impl_shortfall_ticks_x_qty` (double): sum over fills of `(fill_price - arrival_mid) * signed_qty_for_cost`
* `sum_abs_impl_shortfall_ticks_x_qty` (double)
* `denom_qty_for_impl_shortfall` (int64): total absolute qty used for averages
* `posted_qty` (int64): qty you attempted to rest (limits excluding market/IOC)
* `executed_posted_qty` (int64)
* `canceled_posted_qty` (int64)

## Drawdown / returns

* `equity_peak_ticks` (double): running peak of `total_pnl_ticks`
* `max_drawdown_ticks` (double): most negative `(total_pnl_ticks - equity_peak_ticks)`
* `bucket_start_ts_us` (int64)
* `last_marked_pnl_ticks` (double)
* `returns_series` (vector<double>) per `return_bucket_us`

## Quote context for marking

* `last_mid_ticks` (int64, optional if mid unknown)

## Per-order arrival cache (for slippage & fill ratio)

Map by your order id:

* `arrival_mid_ticks` (int64)
* `arrival_ts_us` (int64)
* `intended_qty` (int32)
* `posted_qty_for_order` (int32)
* `is_maker_candidate` (bool)  // true for non-IOC limit orders
* `side` (BUY/SELL)

---

# Time series (for plotting)

Vectors aligned by timestamp:

* `ts_us` (vector<int64>)
* `total_pnl_ticks_series` (vector<double>)
* `realized_pnl_ticks_series` (vector<double>)
* `unrealized_pnl_ticks_series` (vector<double>)
* `position_series` (vector<int64>)
* `mid_series` (vector<int64>)  // optional if you mark at MID

Append to these on each mark or per event (your choice).

---

# Event inputs (what Metrics listens to)

### 1) Order lifecycle hooks

* `on_order_placed(order_id, side, price_ticks, qty, ts_us, is_ioc_or_market, arrival_mid_ticks)`

  * Cache `arrival_mid_ticks`, `arrival_ts_us`, `intended_qty`, `side`.
  * If **resting candidate** (not IOC/market), increase `posted_qty` by `qty` and flag `is_maker_candidate=true`.
* `on_order_canceled(order_id, remaining_qty, ts_us)`

  * If order had `is_maker_candidate=true`, increase `canceled_posted_qty` by `remaining_qty`. Remove from cache.

### 2) Fills (core accounting)

* `on_fill(trade_id, order_id, side, is_maker, fill_price_ticks, fill_qty, ts_us)`

  * **Fees/Rebates**:

    * If `is_maker`: `fees_ticks += maker_rebate_ticks * fill_qty`
    * Else (taker): `fees_ticks -= taker_fee_ticks * fill_qty`
  * **Inventory/P&L accounting** (robust average-cost logic):

    * Let `pos_old = position`, `p = fill_price_ticks`, `q = fill_qty`, `sgn = +1 for BUY, -1 for SELL`.
    * If `pos_old == 0` or `sgn == sign(pos_old)` (expanding position):

      * `position += sgn * q`
      * Recompute `avg_entry_price_ticks` as weighted average:

        * `avg = (avg * abs(pos_old) + p * q) / (abs(pos_old) + q)`
    * Else (reducing or flipping):

      * `closed = min(q, abs(pos_old))`
      * If `pos_old > 0` (you were long, now selling):
        `realized_pnl_ticks += (p - avg_entry_price_ticks) * closed`
      * If `pos_old < 0` (you were short, now buying):
        `realized_pnl_ticks += (avg_entry_price_ticks - p) * closed`
      * Update `position += sgn * q`
      * If you **flipped** (abs(position) > 0 after crossing zero):

        * Set `avg_entry_price_ticks = p` for the new residual position direction
  * **Execution quality**:

    * If we have `arrival_mid_ticks` in cache for `order_id`, compute **implementation shortfall** per filled qty:

      * For BUY: `shortfall = (p - arrival_mid_ticks)`
      * For SELL: `shortfall = (arrival_mid_ticks - p)`
      * Accumulate:

        * `sum_impl_shortfall_ticks_x_qty += shortfall * q`
        * `sum_abs_impl_shortfall_ticks_x_qty += abs(shortfall) * q`
        * `denom_qty_for_impl_shortfall += q`
    * If `is_maker == true`, increase `executed_posted_qty += q`
    * Increase `gross_traded_qty += q`
  * (Do **not** remove order from cache here unless it is fully done and you track remaining qty; otherwise remove on cancel/complete as appropriate.)

### 3) Marks (unrealized P&L & returns)

* `on_quote(best_bid_ticks, best_ask_ticks, ts_us)`

  * Compute `mid_ticks` per `mark_method`.
  * `last_mid_ticks = mid_ticks`
  * `unrealized_pnl_ticks = position * (mid_ticks - avg_entry_price_ticks)`  (works for long/short with signed `position`)
  * `total_pnl_ticks = realized_pnl_ticks + fees_ticks + unrealized_pnl_ticks`
  * Append to time series (ts, pnl, position, mid) if sampling each event.
  * Returns bucketing: if `ts_us - bucket_start_ts_us >= return_bucket_us`:

    * `ret = total_pnl_ticks - last_marked_pnl_ticks`
    * Push `ret` to `returns_series`, set `last_marked_pnl_ticks = total_pnl_ticks`, advance `bucket_start_ts_us = ts_us`.
  * **Drawdown**:

    * If `total_pnl_ticks > equity_peak_ticks`: `equity_peak_ticks = total_pnl_ticks`
    * Else: `dd = total_pnl_ticks - equity_peak_ticks` (negative) and `max_drawdown_ticks = min(max_drawdown_ticks, dd)`

---

# Derived outputs (computed on request)

### P&L & risk

* `get_realized_pnl_ticks()`, `get_unrealized_pnl_ticks()`, `get_total_pnl_ticks()`
* `get_position()`, `get_avg_entry_price_ticks()`
* `get_max_drawdown_ticks()`

### Execution quality & activity

* `get_fill_ratio()` = `executed_posted_qty / max(1, posted_qty)`
* `get_impl_shortfall_avg_ticks()` = `sum_impl_shortfall_ticks_x_qty / max(1, denom_qty_for_impl_shortfall)`
* `get_impl_shortfall_abs_avg_ticks()` (same with abs)
* `get_maker_taker_breakdown()` → {maker_qty, taker_qty, fees_ticks}

### Returns stats

* `get_sharpe(scale_factor)`

  * Compute mean/std of `returns_series`, multiply by `scale_factor` (e.g., per-second to per-day).
* `get_volatility()` (std of returns)
* `get_summary()`

  * Single struct aggregating the main KPIs (pnl, sharpe, drawdown, fill ratio, slippage, activity counts)

### Time series accessors

* `get_equity_curve()` → (ts, total_pnl_ticks)
* `get_position_series()`, `get_mid_series()`, etc.

---

# Lifecycle methods

* `reset()` → clear all state and series
* `finalize()` → ensure last bucket is closed; compute any lazy stats
* `export_csv(paths)` (optional) → write series and summary for plotting
* `snapshot(ts_us)` → cheap struct with current metrics (for live dashboards)

---

# Notes & conventions

* **Units:** keep everything in **ticks** internally (prices, pnl, fees). Convert to currency only for I/O (`* tick_size`).
* **Partial fills:** accounted naturally—each `on_fill` adjusts position, realized P&L, fees.
* **Short positions:** handled by the signed `position` and the “opposite” realized P&L formula when reducing.
* **Maker vs taker:** pass `is_maker` from your matching logic (incoming aggressive order is taker; resting order is maker).
* **Fill ratio denominator:** only count qty from **resting** limit orders (`posted_qty`), not IOC/market qty.
* **Implementation shortfall reference:** use **arrival mid** at **order placement** time; store per order id in the arrival cache.

---

# Minimal method surface (summary)

* Config: `set_config`, `get_config`
* Orders: `on_order_placed`, `on_order_canceled`
* Fills: `on_fill`
* Quotes/Marks: `on_quote`
* Series/Stats: `get_*` accessors above
* Lifecycle: `reset`, `finalize`, optional `export_csv`, `snapshot`

---

With this spec you can implement `Metrics` once and forget it. Feed it **fills** from your `OrderBook` and **quotes** (best bid/ask) from your book snapshots; it will produce everything you need to evaluate strategies and showcase results.
