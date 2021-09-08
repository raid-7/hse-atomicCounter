#include "counter.hpp"
#include "assert.hpp"

#include <thread>


static void test_initial_value() {
    std::cout << "Running test " << __func__ << std::endl;

    counter c{3};

    ensure(eq, 0u, c.get());
}

static void test_inc() {
    std::cout << "Running test " << __func__ << std::endl;

    counter c{3};

    c.inc();
    c.inc();

    ensure(eq, 2u, c.get());

    c.inc();
    c.inc();

    ensure(eq, 4u, c.get());
}

static void test_concurrent_monotonic(uint32_t counter_sz, uint32_t num_inc_threads, uint32_t num_check_threads, uint32_t num_iterations) {
    std::cout << "Running test " << __func__ << std::endl;

    std::vector<std::thread> threads;
    counter c{counter_sz};
    std::atomic<uint32_t> atomic_c{0};

    for (uint32_t i = 0; i < num_inc_threads; i++)
        threads.emplace_back(
            [&c, num_iterations]() {
                for (uint32_t j = 0; j < num_iterations; j++) {
                   c.inc();
                }
            }
        );

    for (uint32_t i = 0; i < num_check_threads; i++)
        threads.emplace_back(
            [&c, &atomic_c, num_inc_threads, num_iterations]() {
                for (uint32_t j = 0; j < num_iterations * 20; j++) {
                    uint32_t max_observed = atomic_c.load(std::memory_order_relaxed);
                    uint32_t cur = c.get();

                    ensure(lte, max_observed, cur);

                    while (cur > max_observed && !atomic_c.compare_exchange_weak(max_observed, cur, std::memory_order_relaxed));

                    if (cur == num_iterations * num_inc_threads)
                        break;
                }
            }
        );

    for (auto& thread : threads) {
        thread.join();
    }

    ensure(eq, num_iterations * num_inc_threads, c.get());
}

static void test_concurrent_inc_with_atomic_lower_bound(uint32_t counter_sz, uint32_t num_threads, uint32_t num_iterations) {
    std::cout << "Running test " << __func__ << std::endl;

    std::vector<std::thread> threads;
    counter c{counter_sz};
    std::atomic<uint32_t> atomic_c{0};

    for (uint32_t i = 0; i < num_threads; i++)
        threads.emplace_back(
            [&c, &atomic_c, num_iterations]() {
                for (uint32_t j = 0; j < num_iterations; j++) {
                    for (int k = 0; k < 3; k++) {
                        c.inc();
                        atomic_c.fetch_add(1, std::memory_order_relaxed);
                    }

                    uint32_t lower_bound = atomic_c.load(std::memory_order_relaxed);
                    uint32_t cur = c.get();
                    ensure(lte, lower_bound, cur);
                }
            }
        );

    for (auto& thread : threads) {
        thread.join();
    }
}

static void test_concurrent_inc_with_atomic_upper_bound(uint32_t counter_sz, uint32_t num_threads, uint32_t num_iterations) {
    std::cout << "Running test " << __func__ << std::endl;

    std::vector<std::thread> threads;
    counter c{counter_sz};
    std::atomic<uint32_t> atomic_c{0};

    for (uint32_t i = 0; i < num_threads; i++)
        threads.emplace_back(
            [&c, &atomic_c, num_iterations]() {
                for (uint32_t j = 0; j < num_iterations; j++) {
                    for (int k = 0; k < 3; k++) {
                        atomic_c.fetch_add(1, std::memory_order_relaxed);
                        c.inc();
                    }

                    uint32_t cur = c.get();
                    uint32_t upper_bound = atomic_c.load(std::memory_order_relaxed);
                    ensure(gte, upper_bound, cur);
                }
            }
        );

    for (auto& thread : threads) {
        thread.join();
    }
}


int main() {
    test_initial_value();
    test_inc();

    test_concurrent_monotonic(3, 3, 3, 16000000);
    test_concurrent_inc_with_atomic_lower_bound(3, 3, 5000000);
    test_concurrent_inc_with_atomic_upper_bound(3, 3, 5000000);

    test_concurrent_monotonic(5, 8, 4, 8000000);
    test_concurrent_inc_with_atomic_lower_bound(3, 8, 2000000);
    test_concurrent_inc_with_atomic_upper_bound(3, 8, 2000000);

    return 0;
}
