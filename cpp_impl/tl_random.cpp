#include "tl_random.hpp"

#include<memory>
#include<atomic>
#include<random>

static thread_local std::unique_ptr<std::minstd_rand> tl_random;
static std::atomic<int> tl_random_init{10};

static std::minstd_rand& get_tl_random() {
    // initialize random with different seeds for different threads
    // to reduce contention on registers of the counter

    if (!tl_random) {
        tl_random = std::make_unique<std::minstd_rand>(tl_random_init.fetch_add(1, std::memory_order_relaxed));
    }
    return *tl_random;
}

uint32_t next_random(uint32_t bound) {
    // we cannot reuse distribution because it is not thread-safe
    // we cannot store thread-local distribution because bounds may change

    return std::uniform_int_distribution<>(0, bound)(get_tl_random());
}
