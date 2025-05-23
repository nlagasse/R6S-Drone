#ifndef CTRLGPIO_H
#define CTRLGPIO_H

//gpio sys directory
#define pinPath "/sys/class/gpio"
#define pinExport "/export"
#define pinUnExport "/unexport"
#define pinValue "/value"
#define pinDir "/direction"
#define pinEdge "/edge"

//pin values
#define dirIn "in"
#define dirOut "out"
#define activehi "HIGH"
#define activelo "LOW"
#define hi "1"
#define lo "0"
//pwm  guh
#define pinPwmPrefix "/sys/class/pwm/pwmchip0"
#define pinPwmExport pinPwmPrefix "/export"
#define pinPwmUnexport pinPwmPrefix "/unexport"
#define pinPwmPeriod pinPwmPrefix "/pwm%d/period"
#define pinPwmDutyCycle pinPwmPrefix "/pwm%d/duty_cycle"
#define pinPwmEnable pinPwmPrefix "/pwm%d/enable"

//gpio pin helper functions

// static int readNwriteFile(const char *fname, const char *wdata);

int gpioExport(int gpio_pin);

int gpioUnExport(int gpio_pin);

int setDirection(int gpio_pin, const char* dir);

int setValue(int gpio_pin, const char* value);

int setEdge(int gpio_pin, const char* edge);

int pinFdtoVal(int gpio_pin);

//pwm helper functions

int pwmExport(int pwm_pin);

int pwmUnExport(int pwm_pin);

int SetPeriod(int pwm_val, unsigned int period);

int SetDutyCycle(int pwm_val, unsigned int duty_cycle);

int pwmEnable(int pwm_pin);

int pwmDisable(int pwm_pin);

//pwm controllor

void pwmController(int pwm_val, unsigned int freq, unsigned int duty_cycle);

#endif