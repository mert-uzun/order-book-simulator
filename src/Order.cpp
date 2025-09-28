#include "../include/Order.h"
#include "../include/IdGenerator.h"

Order::Order(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp) {
    this->id = IdGenerator::getNext();
    this->isBuy = isBuy;
    this->isActive = true;
    this->priceTick = priceTick;
    this->quantity = quantity;
    this->tsCreatedUs = timestamp;
    this->tsLastUpdateUs = timestamp;
}