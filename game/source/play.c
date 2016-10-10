#include "mem.h"
#include "gpio.h"
#include "dummy.h"
#include "random.h"
#include "wait.h"
#include "play.h"


typedef struct {
    unsigned input_pin;
    unsigned output_pin;
} PlayPin;


#define PLAY_PINS_NO 4
#define STATUS_PINS_NO 3
#define MELODY_LENGHT 16

unsigned read_keypress(volatile unsigned *gpio, const PlayPin const play_pins[]) {
    int idx;
    while (1) {
        for (idx = 0; idx < PLAY_PINS_NO; idx++) {
            PlayPin play_pin = play_pins[idx];
            int is_pressed = !read_pin(gpio, play_pin.input_pin);
            if (is_pressed) {
                set_pin(gpio, play_pin.output_pin);
                while(!read_pin(gpio, play_pin.input_pin));
                clear_pin(gpio, play_pin.output_pin);
                return idx;
            }
        }
    }
}

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

void compose_melody(int * melody) {
    volatile unsigned * const random_generator = setup_random();
    start_random(random_generator);
    unsigned seed = get_random(random_generator);
    teardown_random(random_generator);
    int note_idx;
    for (note_idx = 0; note_idx < MELODY_LENGHT; note_idx++) {
        melody[note_idx] = seed & 0x3;
        seed >>= 2;
    }
}

void play() {
    volatile unsigned * const timer = setup_timer();
    volatile unsigned * const gpio = setup_gpio();

    /* PINS CONFIGURATION */
    /* PULL_UP is only available on pins 1-8 */
    const PlayPin const play_pins[PLAY_PINS_NO] = {
        { .input_pin = 2, .output_pin = 17},
        { .input_pin = 4, .output_pin = 27},
        { .input_pin = 3, .output_pin = 22},
        { .input_pin = 8, .output_pin = 10}
    };

    const unsigned status_pins[STATUS_PINS_NO] = {9, 11, 24};
    const unsigned wait_pin = status_pins[0];
    const unsigned success_pin = status_pins[1];
    const unsigned failure_pin = status_pins[2];

    /* PINS SETUP */
    int idx;
    for (idx = 0; idx < PLAY_PINS_NO; idx++) {
        PlayPin play_pin = play_pins[idx];
        set_mode(gpio, play_pin.input_pin, GPIO_MODE_INPUT);
        set_pull_up_down(gpio, play_pin.input_pin, GPIO_PULL_UP);
        set_mode(gpio, play_pin.output_pin, GPIO_MODE_OUTPUT);
    }

    for (idx = 0; idx < STATUS_PINS_NO; idx++) {
        set_mode(gpio, status_pins[idx], GPIO_MODE_OUTPUT);
    }

    /* HEALTH CHECK */
    for (idx = 0; idx < PLAY_PINS_NO; idx++) {
        PlayPin play_pin = play_pins[idx];
        set_pin(gpio, play_pin.output_pin);
    }
    for (idx = 0; idx < STATUS_PINS_NO; idx++) {
        set_pin(gpio, status_pins[idx]);
    }
    wait(timer, 3000);
    for (idx = 0; idx < PLAY_PINS_NO; idx++) {
        PlayPin play_pin = play_pins[idx];
        clear_pin(gpio, play_pin.output_pin);
    }
    for (idx = 0; idx < STATUS_PINS_NO; idx++) {
        clear_pin(gpio, status_pins[idx]);
    }
    wait(timer, 1000);


    const int melody[MELODY_LENGHT];
    compose_melody((int*)melody);

    int level = 1;
    /* START GAME */
    while (level <= MELODY_LENGHT) {
        /* Play pattern */
        int note_idx;
        for (note_idx = 0; note_idx < level; note_idx++) {
            int note = melody[note_idx];
            PlayPin note_pin = play_pins[note];
            if (note_idx + 1 == level) {
                set_pin(gpio, note_pin.output_pin);
                wait(timer, 500);
                clear_pin(gpio, note_pin.output_pin);
            } else {
                flash_pin(gpio, timer, note_pin.output_pin, 1, 500);
            }
        }

        /* wait for input */
        set_pin(gpio, wait_pin);
        for (note_idx = 0; note_idx < level; note_idx++) {
            int expected_note = melody[note_idx];
            int played_note = read_keypress(gpio, play_pins);
            if (played_note == expected_note) {
                continue;
            } else {
                clear_pin(gpio, wait_pin);
                flash_pin(gpio, timer, failure_pin, 5, 250);
                return; /* GAME OVER */
            }
        }
        clear_pin(gpio, wait_pin);
        /* all notes correct */
        flash_pin(gpio, timer, success_pin, 1, 250);
        level++;
    }

    teardown_gpio(gpio);
    teardown_timer(timer);
}
