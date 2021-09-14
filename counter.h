#pragma once

#include "tl_random.h"

#include<stdatomic.h>
#include<inttypes.h>
#include<malloc.h>


#ifndef COUNTER_K
#define COUNTER_K 5
#endif


typedef struct counter {
    atomic_uint registers[COUNTER_K];
} counter;


static void counter_init(counter* cnt) {
    for (uint32_t i = 0; i < COUNTER_K; i++) {
        atomic_uint* reg = &cnt->registers[i];
        atomic_init(reg, 0u);
    }
}

static uint32_t get(counter* cnt) {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < COUNTER_K; i++) {
        sum += atomic_load(&cnt->registers[i]);
    }
    return sum;
}

static void inc(counter* cnt) {
    uint32_t register_id = next_random_uint(COUNTER_K);
    atomic_fetch_add(&cnt->registers[register_id], 1);
}
