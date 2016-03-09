#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>

#define PERI_BASE		0x3F000000 // Peripheral memory space
#define GPIO_BASE		(PERI_BASE + 0x200000) // GPIO pins offset inside of the peripheral memory space

#define BLOCK_SIZE	(4*1024)

int main(int argc, char *argv[])
{
	int index = 0;
	int size = argc - 1;

	volatile unsigned* gpio;
	gpio = (volatile unsigned*)map_memory(GPIO_BASE, BLOCK_SIZE);

	set_mode(gpio, 23, 1);

	while(1 == 1)
	{
		set_value(gpio, 23, 1);
		sleep(1);
		
		clear_pin(gpio, 23);
		sleep(1);
	}

	return 0;
}

