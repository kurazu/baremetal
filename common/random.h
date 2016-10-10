#define RANDOM_BASE 0x20104000
#define RANDOM_NORMAL_SPEED 0x1

volatile unsigned * setup_random();
void teardown_random(volatile unsigned * const random_generator);

void start_random(volatile unsigned * const random_generator);
unsigned get_random(volatile unsigned * const random_generator);
