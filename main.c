#include <stdio.h>

#include "pwm.h"

int main(int argc, char *argv[])
{
    struct pwm_chip pwmc;

    int num = PWM_num_channels(&pwmc);

    printf("There are %d pwm channels\n", num);

    PWM_export_channel(&pwmc, 0);

    PWM_set_dutycycle(&pwmc, 0, 1000000);
    PWM_set_period(&pwmc, 0, 1000000);
    PWM_enable(&pwmc, 0);


    return 0;
}