# baremetal
RaspberryPI baremetal examples

## common
The directory contains common modules for operating RPi from C.

* ```dummy``` - empty function (in a separate compilation unit to avoid compiler optimization)
* ```gpio``` - operate on RPi's GPIO
* ```kernel.ld``` - linker script to build baremetal RPi images
* ```main``` - bootstrap code 
* ```mem``` - memory mapping
* ```play``` - template of the actual program
* ```random``` - operate on the hardware random number generator
* ```wait``` - operate on hardware timer

Modules named ```*.linux.*``` are specific to Linux environment.
Modules name ```*.baremetal.*``` are specific to baremetal environment.

There is also a ```gen_makefile.py``` script that generates ```Makefile``` content for compiling a program that uses those modules in both versions (Linux and baremetal). The script was used to generate ```Makefile```s from examples.

## test
Simple tester program. Waits for button to be pressed. While button is held down, blinks a LED.

## game
Game of memory - generates a random "melody". The "melody" is played in increments. The user needs to repeat the gradually longer "melody".
