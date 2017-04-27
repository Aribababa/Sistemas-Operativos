/*
*	Basic test for GPIO Driver
*	Authors:
*		Ramiro Manuel Rubio
*		Victor Ramirez
*		Rafael López
*/		

#include "gpio_driver.h"

int main(void)
{
	PWM_period_us(2000000);
	PWM(50);
}
