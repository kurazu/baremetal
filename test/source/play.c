#include "mem.h"
#include "gpio.h"
#include "wait.h"
#include "play.h"


void flash_pin(
    volatile unsigned *gpio, volatile unsigned *timer,
    unsigned pin, unsigned times, unsigned long long miliseconds
) {
    unsigned iteration;
    for (iteration = 0; iteration < times; iteration++) {
        set_pin(gpio, pin);
        wait(timer, miliseconds);
        clear_pin(gpio, pin);
        wait(timer, miliseconds);
    }
}

void play() {
    volatile unsigned * const timer = setup_timer();
    volatile unsigned * const gpio = setup_gpio();

    const unsigned led_pin = 14;
    const unsigned switch_pin = 4;

    set_mode(gpio, led_pin, GPIO_MODE_OUTPUT);
    set_mode(gpio, switch_pin, GPIO_MODE_INPUT);
    set_pull_up_down(gpio, switch_pin, GPIO_PULL_UP);

    int is_pressed = 0;
    while (!is_pressed) {
        is_pressed = !read_pin(gpio, switch_pin);
    }

    while (is_pressed) {
        set_pin(gpio, led_pin);
        wait(timer, 1000);
        clear_pin(gpio, led_pin);
        wait(timer, 1000);
        is_pressed = !read_pin(gpio, switch_pin);
    }

    teardown_gpio(gpio);
    teardown_timer(timer);
}
