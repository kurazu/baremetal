#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

volatile unsigned * setup_gpio();
void teardown_gpio(volatile unsigned * const gpio);

#define GPIO_MODE_INPUT 0x0
#define GPIO_MODE_OUTPUT 0x1

#define GPIO_PULL_OFF 0x0
#define GPIO_PULL_DOWN 0x1
#define GPIO_PULL_UP 0x2

void set_mode(volatile unsigned * const gpio, const unsigned pin, const unsigned mode);
void set_pin(volatile unsigned * const gpio, const unsigned pin);
void clear_pin(volatile unsigned * const gpio, const unsigned pin);
void set_pull_up_down(volatile unsigned * const gpio, const unsigned pin, const unsigned mode);
int read_pin(volatile unsigned * const gpio, const unsigned pin);
