#pragma once

class IdGenerator {
    private:
        static int64_t current = 1;
    public:
        static int64_t getNext();
};