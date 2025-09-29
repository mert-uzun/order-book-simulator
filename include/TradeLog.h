#include <list>
#include "Trade.h"
class TradeLog {
    private:
        std::list<Trade> trades;
    public:
        TradeLog();
        int64_t add_trade(Trade);
        void show_trades();
};