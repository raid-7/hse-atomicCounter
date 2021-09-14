#ifdef GMC
#define COUNTER_K 2
#endif

#include "counter.h"

#include<stdio.h>
#include<assert.h>
#include<pthread.h>
#include<stdatomic.h>


static void test_initial_value() {
    counter c;
    counter_init(&c);
    assert(0u == get(&c));
}

static void test_inc() {
    counter c;
    counter_init(&c);

    inc(&c);
    inc(&c);

    assert(2u == get(&c));

    inc(&c);
    inc(&c);

    assert(4u == get(&c));
}


struct descriptor {
    counter* c;
    atomic_uint* atomic_c;
};


static void* run_increments(void* desc_ptr) {
    struct descriptor* desc = (struct descriptor*) desc_ptr;

    inc(desc->c);
    inc(desc->c);
    inc(desc->c);

    return NULL;
}

static void* run_checkers(void* desc_ptr) {
    struct descriptor* desc = (struct descriptor*) desc_ptr;

    for (uint32_t j = 0; j < 3; j++) {
        uint32_t max_observed = atomic_load_explicit(desc->atomic_c, memory_order_seq_cst);
        uint32_t cur = get(desc->c);

        assert(max_observed <= cur);

        while (cur > max_observed && !atomic_compare_exchange_weak(desc->atomic_c, &max_observed, cur));

        if (cur == 6)
            break;
    }

    return NULL;
}

static void test_concurrent_monotonic() {
    counter c;
    counter_init(&c);
    atomic_uint atomic_c;
    atomic_init(&atomic_c, 0);
    pthread_t threads[4];
    struct descriptor desc;

    desc.c = &c;
    desc.atomic_c = &atomic_c;

    pthread_create(&threads[0], NULL, run_increments, &desc);
    pthread_create(&threads[1], NULL, run_increments, &desc);
    pthread_create(&threads[2], NULL, run_checkers, &desc);
    pthread_create(&threads[3], NULL, run_checkers, &desc);

    for (uint32_t i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    assert(6 == get(&c));
}

static void* run_lower_bound_inc_check_thread(void* desc_ptr) {
    struct descriptor* desc = (struct descriptor*) desc_ptr;

    for (uint32_t j = 0; j < 2; j++) {
        inc(desc->c);
        atomic_fetch_add(desc->atomic_c, 1);

        uint32_t lower_bound = atomic_load(desc->atomic_c);
        uint32_t cur = get(desc->c);
        assert(lower_bound <= cur);
    }

    return NULL;
}

static void* run_upper_bound_inc_check_thread(void* desc_ptr) {
    struct descriptor* desc = (struct descriptor*) desc_ptr;

    for (uint32_t j = 0; j < 2; j++) {
        atomic_fetch_add(desc->atomic_c, 1);
        inc(desc->c);

        uint32_t cur = get(desc->c);
        uint32_t upper_bound = atomic_load(desc->atomic_c);
        assert(upper_bound >= cur);
    }

    return NULL;
}

static void test_concurrent_3_symmetric_threads(void* (*thread_func)(void*)) {
    counter c;
    counter_init(&c);
    atomic_uint atomic_c;
    atomic_init(&atomic_c, 0);
    pthread_t threads[3];
    struct descriptor desc;

    desc.c = &c;
    desc.atomic_c = &atomic_c;

    pthread_create(&threads[0], NULL, thread_func, &desc);
    pthread_create(&threads[1], NULL, thread_func, &desc);
    pthread_create(&threads[2], NULL, thread_func, &desc);

    for (uint32_t i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
}

void genmc_initial_value() {
    test_initial_value();
}

void genmc_inc() {
    test_inc();
}

void genmc_concurrent_monotonic() {
    test_concurrent_monotonic();
}

void genmc_with_atomic_lb() {
    test_concurrent_3_symmetric_threads(run_lower_bound_inc_check_thread);
}

void genmc_with_atomic_ub() {
    test_concurrent_3_symmetric_threads(run_upper_bound_inc_check_thread);
}

int main() {
    test_initial_value();
    test_inc();
    test_concurrent_monotonic();
    test_concurrent_3_symmetric_threads(run_lower_bound_inc_check_thread);
    test_concurrent_3_symmetric_threads(run_upper_bound_inc_check_thread);

    return 0;
}
