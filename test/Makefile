ARMGNU ?= /home/kurazu/tmp/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
COPS = -Wall -Werror -nostdlib -nostartfiles -ffreestanding

all: build.linux build.linux/test build.baremetal build.baremetal/test.img

build.linux:
	mkdir build.linux

build.baremetal:
	mkdir build.baremetal

build.linux/play.o: source/play.c source/mem.h source/gpio.h source/wait.h source/play.h
	$(ARMGNU)gcc -c source/play.c -o build.linux/play.o

build.linux/wait.o: source/wait.c source/wait.h source/mem.h
	$(ARMGNU)gcc -c source/wait.c -o build.linux/wait.o

build.linux/gpio.o: source/gpio.c source/gpio.h source/mem.h source/dummy.h
	$(ARMGNU)gcc -c source/gpio.c -o build.linux/gpio.o

build.linux/dummy.o: source/dummy.c source/dummy.h
	$(ARMGNU)gcc -c source/dummy.c -o build.linux/dummy.o

build.linux/mem.o: source/mem.linux.c source/mem.h
	$(ARMGNU)gcc -c source/mem.linux.c -o build.linux/mem.o

build.linux/main.o: source/main.linux.c source/play.h
	$(ARMGNU)gcc -c source/main.linux.c -o build.linux/main.o

build.linux/test: build.linux build.linux/play.o build.linux/wait.o build.linux/gpio.o build.linux/dummy.o build.linux/mem.o build.linux/main.o
	$(ARMGNU)gcc build.linux/play.o build.linux/wait.o build.linux/gpio.o build.linux/dummy.o build.linux/mem.o build.linux/main.o -o build.linux/test

build.baremetal/play.o: source/play.c source/mem.h source/gpio.h source/wait.h source/play.h
	$(ARMGNU)gcc $(COPS) -c source/play.c -o build.baremetal/play.o

build.baremetal/wait.o: source/wait.c source/wait.h source/mem.h
	$(ARMGNU)gcc $(COPS) -c source/wait.c -o build.baremetal/wait.o

build.baremetal/gpio.o: source/gpio.c source/gpio.h source/mem.h source/dummy.h
	$(ARMGNU)gcc $(COPS) -c source/gpio.c -o build.baremetal/gpio.o

build.baremetal/dummy.o: source/dummy.c source/dummy.h
	$(ARMGNU)gcc $(COPS) -c source/dummy.c -o build.baremetal/dummy.o

build.baremetal/mem.o: source/mem.baremetal.c source/mem.h
	$(ARMGNU)gcc $(COPS) -c source/mem.baremetal.c -o build.baremetal/mem.o

build.baremetal/main.o: source/main.baremetal.s
	$(ARMGNU)as source/main.baremetal.s -o build.baremetal/main.o

build.baremetal/test.img: build.baremetal build.baremetal/test.elf
	$(ARMGNU)objcopy build.baremetal/test.elf -O binary build.baremetal/test.img

build.baremetal/test.elf: build.baremetal build.baremetal/play.o build.baremetal/wait.o build.baremetal/gpio.o build.baremetal/dummy.o build.baremetal/mem.o build.baremetal/main.o source/kernel.ld
	$(ARMGNU)ld --no-undefined build.baremetal/play.o build.baremetal/wait.o build.baremetal/gpio.o build.baremetal/dummy.o build.baremetal/mem.o build.baremetal/main.o -Map build.baremetal/kernel.map -T source/kernel.ld -o build.baremetal/test.elf
	$(ARMGNU)objdump -D build.baremetal/test.elf > build.baremetal/test.list

clean:
	rm -rf build.linux build.baremetal

