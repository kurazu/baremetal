#include "gpio.h"
#include "mem.h"
#include "dummy.h"

volatile unsigned * setup_gpio() {
   return setup_memory(GPIO_BASE);
}

void teardown_gpio(volatile unsigned * const gpio) {
    teardown_memory(gpio);
}

void set_mode(volatile unsigned * const gpio, const unsigned pin, const unsigned mode) {
    volatile unsigned * const control_word = gpio + (pin / 10);
    const unsigned bit_offset = (pin % 10) * 3;
    *control_word = ((*control_word) & ~(7 << bit_offset)) | (mode << bit_offset);
}

void set_pin(volatile unsigned * const gpio, const unsigned pin) {
    volatile unsigned * const control_word = gpio + 7 + (pin / 32);
    const unsigned bit_offset = (pin % 32);
    *control_word = 1 << bit_offset;
}

void clear_pin(volatile unsigned * const gpio, const unsigned pin) {
    volatile unsigned * const control_word = gpio + 10 + (pin / 32);
    const unsigned bit_offset = (pin % 32);
    *control_word = 1 << bit_offset;
}

void wait150cycles() {
    unsigned i = 0;
    while (i++ < 150) {
        dummy();
    }
}

void set_pull_up_down(volatile unsigned * const gpio, const unsigned pin, const unsigned mode) {
    volatile unsigned* const pull_up_down_control_word = gpio + 37;
    *pull_up_down_control_word = mode;
    wait150cycles();
    volatile unsigned * const pull_up_down_clock_control_word = gpio + 38 + (pin / 32);
    const unsigned bit_offset = (pin % 32);
    *pull_up_down_clock_control_word = 1 << bit_offset;
    wait150cycles();
    *pull_up_down_control_word = GPIO_PULL_OFF;
    *pull_up_down_clock_control_word = 0;
}

int read_pin(volatile unsigned * const gpio, const unsigned pin) {
    volatile unsigned * const control_word = gpio + 13 + (pin / 32);
    const unsigned bit_offset = (pin % 32);
    return (*control_word) & (1 << bit_offset);
}
