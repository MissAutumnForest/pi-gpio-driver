void set_mode(volatile unsigned *gpio, int pin, int mode)
{
	if(mode == 1)
	{
		// Set pin to output mode
		*(gpio + ((pin)/10)) |=  (1<<(((pin)%10)*3));
	}
	else
	{
		// Set pin to input mode
		*(gpio + ((pin)/10)) &= ~(7<<(((pin)%10)*3));
	}
}


void set_alt(volatile unsigned *gpio, int pin, int a)
{
	*(gpio + (((pin)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((pin)%10)*3));
}



void set_value(volatile unsigned *gpio, int pin, int value)
{
	if(value == 1)
	{
		*(gpio + 7) = 1 << pin;
	}
}

void clear_pin(volatile unsigned *gpio, int pin)
{
	*(gpio + 10) = 1 << pin;
}

volatile unsigned read_value(volatile unsigned *gpio, int pin)
{
	return *(gpio + 13) &= (1<<(pin));
}
