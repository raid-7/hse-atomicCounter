#pragma once

#include<inttypes.h>

#ifdef GMC

#include<stdlib.h>

static uint32_t next_random_uint(uint32_t max) {
    uint_fast32_t base = abs(__VERIFIER_nondet_int());
    return base % max;
}

#else

#include<stdatomic.h>
#include<threads.h>


struct random {
    uint_fast32_t a, b, m, x;
};

static _Thread_local uint_fast32_t tl_random_a = 0;
static _Thread_local uint_fast32_t tl_random_b = 0;
static _Thread_local uint_fast32_t tl_random_m = 0;
static _Thread_local uint_fast32_t tl_random_x = 0;
static atomic_uint_fast32_t tl_random_init_seed;

static void init_tl_random(uint_fast32_t x) {
    tl_random_a = 4096;
    tl_random_b = 150889;
    tl_random_m = 714025;
    tl_random_x = x % tl_random_m;
}

static void ensure_tl_random_initialized() {
    if (tl_random_m == 0) {
        uint_fast32_t seed = atomic_fetch_add_explicit(&tl_random_init_seed, 1u, memory_order_relaxed) + 10;
        init_tl_random(seed);
    }
}

static uint_fast32_t next_random() {
    ensure_tl_random_initialized();
    return tl_random_x = (tl_random_x * tl_random_a + tl_random_b) % tl_random_m;
}

static uint32_t next_random_uint(uint32_t max) {
    uint_fast32_t base = next_random();
    if (max > tl_random_m) {
        base ^= next_random() << 16;
    }
    return base % max;
}

#endif