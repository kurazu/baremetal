#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "mem.h"

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

volatile unsigned * setup_memory(const unsigned address) {
    /* open /dev/mem */
    const int mem_fd = open("/dev/mem", O_RDWR|O_SYNC);

    if (mem_fd < 0) {
       printf("can't open /dev/mem \n");
       exit(-1);
    }

    /* mmap GPIO */
    const void *memory_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      address         //Offset to GPIO peripheral
    );

    close(mem_fd); //No need to keep mem_fd open after mmap

    if (memory_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)memory_map); //errno also set!
      exit(-2);
   }

   // Always use volatile pointer!
   (volatile unsigned *)memory_map;
}

void teardown_memory(volatile unsigned * const memory_map) {
    const int result = munmap((void *)memory_map, BLOCK_SIZE);
    if (result != 0) {
        printf("munmap error %d\n", result);
        exit(-3);
    }
}
