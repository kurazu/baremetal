#include "play.h"
#include "wait.h"
#include "gpio.h"
#include "random.h"
#include "dummy.h"

void play() {
    volatile unsigned * const timer = setup_timer();
    volatile unsigned * const gpio = setup_gpio();
    volatile unsigned * const rng = setup_random();
    start_random(rng);

    // Code goes here.

    teardown_random(rng);
    teardown_gpio(gpio);
    teardown_timer(timer);
}
