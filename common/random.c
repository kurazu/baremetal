#include "mem.h"
#include "random.h"

volatile unsigned * setup_random() {
    return setup_memory(RANDOM_BASE);
}

void teardown_random(volatile unsigned * const random_generator) {
    return teardown_memory(random_generator);
}

void start_random(volatile unsigned * const random_generator) {
    *random_generator = RANDOM_NORMAL_SPEED;
}

int random_has_data(volatile unsigned * const random_generator) {
    volatile unsigned * const random_status_word = random_generator + 1;
    const unsigned status = *random_status_word;
    const unsigned valid_words_count = (status & 0xff000000) >> 24;
    return valid_words_count > 0;
}

unsigned get_random(volatile unsigned * const random_generator) {
    while (!random_has_data(random_generator));
    volatile unsigned * const random_data_word = random_generator + 2;
    return *random_data_word;
}
