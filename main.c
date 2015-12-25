#include <stdio.h>
#include <fcntl.h>
 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
  
#include <unistd.h>

#define BCM2708_PERI_BASE	0x20000000
#define GPIO_BASE		(BCM2708_PERI_BASE + 0x200000) // GPIO Controller memeory space

#define BLOCK_SIZE		(4*1024)
#define GPIO_COUNT		10

#define MODE_IN			7
#define MODE_OUT		1

// #define INP_GPIO(g)		*(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
// #define OUT_GPIO(g)		*(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
// #define SET_GPIO_ALT(g,a)	*(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET  *(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR  *(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0
 
#define GPIO_READ(g)  *(gpio.addr + 13) &= (1<<(g))

// IO Access to the pins
struct bcm2835_peripheral
{
	unsigned long addr_p;
	int mem_fd;
	void *map;
	volatile unsigned int *addr;
};

struct bcm2835_peripheral gpio = {GPIO_BASE};

// Our interactions with the hardware.
int SET_GPIO_MODE(int PIN, char MODE)
{
	volatile unsigned int	GPIN_GROUP = *(gpio.addr + ((PIN)/10));
	unsigned int		GPIN_LOCATION = (((PIN)%10)*3);
	unsigned int		GPIN_VALUE;

	if(MODE == 'I' || MODE == 'i')
	{
		GPIN_VALUE = ~(MODE_IN<<GPIN_LOCATION);
		GPIN_GROUP &= GPIN_VALUE;
	}
	else if (MODE == 'O' || MODE == 'o')
	{
		// We need to set it to be in input mode before output mode.
		GPIN_VALUE = ~(MODE_IN<<GPIN_LOCATION);
		GPIN_GROUP &= GPIN_VALUE;

		// Actually set it output mode.
		GPIN_VALUE = (MODE_OUT<<GPIN_LOCATION);
		GPIN_GROUP |= GPIN_VALUE;
	}
	else
	{
		printf("Wrong MODE Type. Expected I|O\n");
		return -1;
	}

	return 0;
}

// Map the absolute addresses to the user space.
int map_peripheral(struct bcm2835_peripheral *p)
{

	if((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
	{
		printf("Failed to open /dev/mem, try checking permissions.\n");
		return -1;
	}
	
	p->map = mmap(
		NULL,
		BLOCK_SIZE,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		p->mem_fd,
		p->addr_p
	);

	if (p->map == MAP_FAILED)
	{
		perror("Can't map memory!");
		return -1;
	}

	p->addr = (volatile unsigned int *)p->map;

	return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p)
{
	munmap(p->map, BLOCK_SIZE);
	close(p->mem_fd);
}

int main(int argc, char *argv[])
{
	if(map_peripheral(&gpio) == -1)
	{
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		return -1;
	}

	SET_GPIO_MODE(4, 'O');

	while(1)
	{
		GPIO_SET = 1 << 4;
		sleep(1);

		GPIO_CLR = 1 << 4;
		sleep(1);

	}

	return 0;
}

