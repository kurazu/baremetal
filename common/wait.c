#include "wait.h"
#include "mem.h"

volatile unsigned * setup_timer() {
    return setup_memory(TIMER_BASE);
}

void teardown_timer(volatile unsigned * const timer) {
    teardown_memory(timer);
}

unsigned long long get_timer_value(volatile unsigned * const timer) {
    volatile unsigned * const timer_counter_word = timer + 1;
    return *((volatile unsigned long long * const)timer_counter_word);
}

void wait(volatile unsigned * const timer, const unsigned long long miliseconds) {
    const unsigned long long start = get_timer_value(timer);
    const unsigned long long expected = start + miliseconds * 1000;
    while (get_timer_value(timer) < expected);
}
