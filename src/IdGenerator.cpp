#include "../include/IdGenerator.h"

long long IdGenerator::current = 1;
long long IdGenerator::currentTrade = 1;

long long IdGenerator::getNext() {
    return current++;
}

long long IdGenerator::getNextTrade() {
    return currentTrade++;
}