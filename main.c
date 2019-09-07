#include <stdio.h>

#include "pwm.h"

int main(int argc, char *argv[])
{
    struct pwm_chip * pwmc = PWM_get_chip(0);

    int num = PWM_num_channels(pwmc);

    printf("There are %d pwm channels\n", num);

    struct pwm_channel *ch0 = PWM_export_channel(pwmc, 0);

    PWM_channel_set_dutycycle(ch0, 1000000);
    PWM_channel_set_period(ch0, 1000000);
    PWM_channel_enable(ch0);


    return 0;
}