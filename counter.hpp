#include<atomic>
#include<random>
#include<vector>

struct counter {
private:
    std::vector<std::atomic<uint32_t>> registers;
    std::uniform_int_distribution<> distribution;

public:
    counter(uint32_t n);
    uint32_t get();
    void inc();
};
