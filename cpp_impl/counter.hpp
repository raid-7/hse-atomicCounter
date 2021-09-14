#pragma once

#include<atomic>
#include<vector>

struct counter {
private:
    std::vector<std::atomic<uint32_t>> registers;

public:
    explicit counter(uint32_t n);
    uint32_t get();
    void inc();
};
