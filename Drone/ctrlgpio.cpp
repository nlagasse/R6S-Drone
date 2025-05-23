#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "ctrlgpio.h"

static int readNwriteFile(const char *fname, const char *wdata)
{
	int fd;
	fd = open(fname, O_WRONLY | O_NONBLOCK);
	if(fd < 0)
		printf("cannot open file %s...%d\r\n", fname, fd);

	write(fd, wdata, strlen(wdata));
	close(fd);
	return 0;
}

int gpioExport(int gpio_pin)
{
	char gpio_str[4];
	sprintf(gpio_str, "%d" ,gpio_pin);
	return readNwriteFile(pinPath pinExport, gpio_str);
}

int gpioUnExport(int gpio_pin)
{
	char gpio_str[4];
	sprintf(gpio_str, "%d" ,gpio_pin);
	return readNwriteFile(pinPath pinUnExport, gpio_str);
}

int setDirection(int gpio_pin, const char* dir)
{
	char path_str[40];
	sprintf(path_str, "%s/gpio%d%s", pinPath, gpio_pin, pinDir);
	return readNwriteFile(path_str, dir);
}

int setValue(int gpio_pin, const char* value)
{
	char path_str[40];
	sprintf(path_str, "%s/gpio%d%s", pinPath, gpio_pin, pinValue);
	return readNwriteFile(path_str, value);
}

int pinFdtoVal(int gpio_pin)
{
	int fd;
	char fname[40];
	sprintf(fname, "%s/gpio%d%s", pinPath, gpio_pin, pinValue);
	fd = open(fname, O_RDONLY | O_NONBLOCK);
	if(fd < 0)
		printf("could not open file %s...%d\r\n", fname, fd);

	return fd; 
}

int pwmExport(int pwm_pin)
{
    char pwm_str[4];
    sprintf(pwm_str, "%d", pwm_pin);
    return readNwriteFile(pinPwmExport, pwm_str);
}

int pwmUnExport(int pwm_pin)
{
    char pwm_str[4];
    sprintf(pwm_str, "%d", pwm_pin);
    return readNwriteFile(pinPwmUnexport, pwm_str);
}

int pwmSetPeriod(int pwm_val, unsigned int period)
{
    char path_str[40];
    sprintf(path_str, pinPwmPeriod, pwm_val);
    char period_str[20];
    sprintf(period_str, "%u", period);
    return readNwriteFile(path_str, period_str);
}

int pwmSetDutyCycle(int pwm_val, unsigned int duty_cycle)
{
    char path_str[40];
    sprintf(path_str, pinPwmDutyCycle, pwm_val);
    char duty_cycle_str[20];
    sprintf(duty_cycle_str, "%u", duty_cycle);
    return readNwriteFile(path_str, duty_cycle_str);
}

int pwmEnable(int pwm_pin)
{
    char path_str[40];
    sprintf(path_str, pinPwmEnable, pwm_pin);
    return readNwriteFile(path_str, "1");
}

int pwmDisable(int pwm_pin)
{
    char path_str[40];
    sprintf(path_str, pinPwmEnable, pwm_pin);
    return readNwriteFile(path_str, "0");
}

// Function to set PWM for a motor (e.g., motorSpeed(1, 10000, 5000) for motor 1, 10 kHz frequency, 50% duty cycle)
void pwmController(int pwm_pin, unsigned int frequency, unsigned int duty_cycle)
{
    pwmSetPeriod(pwm_pin, frequency);
    pwmSetDutyCycle(pwm_pin, duty_cycle);
    pwmEnable(pwm_pin);
}