#include "../include/IdGenerator.h"

int64_t IdGenerator::current = 1;
int64_t IdGenerator::currentTrade = 1;

int64_t IdGenerator::getNext() {
    return current++;
}

int64_t IdGenerator::getNextTrade() {
    return currentTrade++;
}