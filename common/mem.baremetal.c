#include "mem.h"

volatile unsigned * setup_memory(const unsigned address) {
    return (volatile unsigned *)address;
}

void teardown_memory(volatile unsigned * const memory_map) {

}

