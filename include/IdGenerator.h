#pragma once

#include <cstdint>

class IdGenerator {
    private:
        static long long current;
        static long long currentTrade;
    public:
        static long long getNext();
        static long long getNextTrade();
};