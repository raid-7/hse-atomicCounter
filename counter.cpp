#include "counter.hpp"

#include<memory>


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

counter::counter(uint32_t n)
	:registers(n), distribution(0, n - 1) {
	for (auto& reg : registers) {
		atomic_init(&reg, 0);
	}
}

uint32_t counter::get() {
	uint32_t sum = 0;
	for (uint32_t i = 0; i < registers.size(); i++) {
		sum += registers[i].load();
	}
	return sum;
}

void counter::inc() {
	int register_id = distribution(get_tl_random());
	registers[register_id].fetch_add(1);
}
