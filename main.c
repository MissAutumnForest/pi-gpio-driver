#include <stdio.h>
#include <fcntl.h>
 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
  
#include <unistd.h>

#define PERI_BASE		0x3F000000 // Peripheral memory space
#define GPIO_BASE		(PERI_BASE + 0x200000) // GPIO pins offset inside of the peripheral memory space

#define INP_GPIO(g)		*(gpio + ((g)/10)) &= ~(7<<(((g)%10)*3)) // Set GPIO pin "g" to input mode
#define OUT_GPIO(g)		*(gpio + ((g)/10)) |=  (1<<(((g)%10)*3)) // Set GPIO pin "g" to output mode
#define SET_GPIO_ALT(g,a)	*(gpio + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3)) // Set GPIO pin "g" to alternate mode "a"
#define GPIO_SET		*(gpio + 7) // Set the value of pin (sets if 1 ignores if 0) Format: 1 << "Pin #) EX: GPIO_SET = 1 << 23
#define GPIO_CLR		*(gpio + 10) // Clears the value of pin (Sets if 1 ignores if 0) Format: 1 << "Pin #) EX: GPIO_CLR = 1 << 23
#define GPIO_READ(g)		*(gpio + 13) &= (1<<(g)) // Reads the value of pin "g"


#define BLOCK_SIZE	(4*1024)

int mem_fd;
char *gpio_mem, *gpio_map;

volatile unsigned *gpio;

int main(int argc, char *argv[])
{
	if((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
	{
		printf("Failed to open /dev/mem, try checking permissions.\n");
		return -1;
	}
	
	gpio_map = mmap(
		NULL,
		BLOCK_SIZE,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		mem_fd,
		GPIO_BASE
	);

	if (gpio_map == MAP_FAILED)
	{
		perror("Can't map memory!");
		return -1;
	}

	gpio = (volatile unsigned *)gpio_map;

	OUT_GPIO(23);

	while(1 == 1)
	{
		GPIO_SET = 1 << 23;
		sleep(1);
		
		GPIO_CLR = 1 << 23;
		sleep(1);
	}

	return 0;
}

