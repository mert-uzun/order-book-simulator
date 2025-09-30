#include "../include/Order.h"
#include "../include/IdGenerator.h"

const double Order::tick_size = 0.001;

Order::Order(bool isBuy, int64_t priceTick, int quantity, int64_t timestamp) {
    this->id = IdGenerator::getNext();
    this->isBuy = isBuy;
    this->isActive = true;
    this->priceTick = priceTick;
    this->quantity = quantity;
    this->tsCreatedUs = timestamp;
    this->tsLastUpdateUs = timestamp;
}

Order::Order(bool isBuy, int quantity, int64_t timestamp) {
    this->id = IdGenerator::getNext();
    this->isBuy = isBuy;
    this->isActive = true;
    this->priceTick = -1; // Indicating this is a market or cancel (IOC) order
    this->quantity = quantity;
    this->tsCreatedUs = timestamp;
    this->tsLastUpdateUs = timestamp;
}