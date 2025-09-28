#include "../include/Trade.h"
#include "../include/Order.h"
#include "../include/IdGenerator.h"

int64_t Trade::current = 1;
int64_t Trade::currentTrade = 1;

Trade::Trade() : tradeId(-1 * IdGenerator::getNextTrade()), buyOrderId(-1), sellOrderId(-1), priceTick(-1), quantity(-1), timestampUs(-1) {} {}

Trade::Trade(int64_t buyOrderId, int64_t sellOrderId, int64_t priceTick, int quantity, int64_t timestampUs) :
    tradeId(IdGenerator::getNextTrade()), buyOrderId(buyOrderId), sellOrderId(sellOrderId), priceTick(priceTick), quantity(quantity), timestampUs(timestampUs) {} 