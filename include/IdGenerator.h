#pragma once

class IdGenerator {
    private:
        static int64_t current;
        static int64_t currentTrade;
    public:
        static int64_t getNext();
        static int64_t getNextTrade();
};