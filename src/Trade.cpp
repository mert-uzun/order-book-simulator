#include "../include/Trade.h"
#include "../include/IdGenerator.h"


Trade::Trade() : tradeId(-1 * IdGenerator::getNextTrade()), buyOrderId(-1), sellOrderId(-1), priceTick(-1), quantity(-1), timestampUs(-1) {}

Trade::Trade(long long buyOrderId, long long sellOrderId, long long priceTick, int quantity, long long timestampUs) :
    tradeId(IdGenerator::getNextTrade()), buyOrderId(buyOrderId), sellOrderId(sellOrderId), priceTick(priceTick), quantity(quantity), timestampUs(timestampUs) {} 