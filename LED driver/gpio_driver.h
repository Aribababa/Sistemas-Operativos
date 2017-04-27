#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void led_on(void);
        /* Prende el LED asociado con el driver */

void led_off(void);
        /* Apaga el LED asociado con el driver */

void PWM(unsigned long duty_cycle);
        /* Genera una señal de PWM*/

void PWM_period_us(unsigned long _period);
        /* Coloca un nuevo periodo al PWM */

unsigned long period = 1000000;   /* El periodod actual del PWM*/
                                  /* Por default el periodo es de 1 segundo */

void led_on(void)
{
        signed int fd, ret;
        fd = open("/dev/gpio_driver", O_RDWR);

        if (fd < 0)
        {
        perror("Failed to open the device...");
    }

    ret = write(fd, "00", 1);
    if (ret < 0){
        perror("Failed to write the message to the device.");
   }
}


void led_off(void)
{
        signed int fd, ret;
        fd = open("/dev/gpio_driver", O_RDWR);

        if (fd < 0)
        {
        perror("Failed to open the device...");
    }

    ret = write(fd, "11", 1);
    if (ret < 0){
        perror("Failed to write the message to the device.");
   }
}

void PWM(unsigned long duty_cycle)
{
        duty_cycle = duty_cycle* 10000;
        while(1)
        {
                led_off();
                usleep(duty_cycle);

                led_on();
                usleep(period - duty_cycle);
        }
}


void PWM_period_us(unsigned long _period)
{
    period=_period;
}
