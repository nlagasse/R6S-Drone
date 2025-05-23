#ifndef MOTOR_H
#define MOTOR_H

#define GPIO7 "/sys/class/gpio/gpio7"
#define GPIO8 "/sys/class/gpio/gpio8"
#define GPIO9 "/sys/class/gpio/gpio9"
#define GPIO10 "/sys/class/gpio/gpio10"

//pwm pins

#define GPIO12 "/sys/class/gpio/gpio12"
#define GPIO13 "/sys/class/gpio/gpio13"

//pin values
#define dirIn "in"
#define dirOut "out"
#define hi "1"
#define lo "0"

void motorLeftFWD();

void motorRightFWD();

void driveForward();

void driveReverse();

void stop();

#endif