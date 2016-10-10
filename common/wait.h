#define TIMER_BASE 0x20003000

volatile unsigned * setup_timer();
void teardown_timer(volatile unsigned * const timer);
void wait(volatile unsigned * const timer, const unsigned long long miliseconds);
unsigned long long get_timer_value(volatile unsigned * const timer);
