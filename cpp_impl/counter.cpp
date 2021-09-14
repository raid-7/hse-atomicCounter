#include "tl_random.hpp"
#include "counter.hpp"

counter::counter(uint32_t n)
	:registers(n) {
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
	int register_id = next_random(registers.size());
	registers[register_id].fetch_add(1);
}
