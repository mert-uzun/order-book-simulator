#include "../include/IdGenerator.h"

IdGenerator::getNext() {
    return current++;
}

IdGenerator::getNextTrade() {
    return currentTrade++;
}