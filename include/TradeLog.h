#include <list>
#include "Trade.h"
class TradeLog {
    private:
        std::list<Trade> trades;
    public:
        TradeLog();
        int64_t add_trade(int64_t buyId, int64_t sellId, int64_t priceTick, int quantity, int64_t timestampUs);
        void show_trades();
};