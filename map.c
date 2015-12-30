#include <stdio.h>
#include <fcntl.h>
 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
   
#include <unistd.h>

volatile unsigned* map_memory(off_t offset, size_t length)
{
	int mem_fd;
	volatile unsigned char* gpio_map;

	if((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0)
	{
		printf("Failed to open /dev/mem, try checking permissions.\n");
		return;
	}
	
	gpio_map = mmap(
		NULL,
		length,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		mem_fd,
		offset
	);

	if (gpio_map == MAP_FAILED)
	{
		perror("Can't map memory!");
		return;
	}

	return (volatile unsigned *)gpio_map;
}
