#include <stdio.h>

#include "pwm.h"

int main(int argc, char *argv[])
{
    struct pwm_chip * pwmc = PWM_get_chip(0);

    int num = PWM_num_channels(pwmc);
    printf("There are %d pwm channels\n", num);

    struct pwm_channel *ch0 = PWM_export_channel(pwmc, 0);

    struct pwm_params params;
    params.duty_cycle   = 1000000;
    params.period       = 500000;
    params.enable       = 1;

    PWM_update_channel(ch0, &params, PWM_DUTYCYCLE_BIT | PWM_PERIOD_BIT | PWM_ENABLE_BIT);

    /*PWM_channel_set_dutycycle(ch0, 1000000);
    PWM_channel_set_period(ch0, 1000000);
    PWM_channel_enable(ch0);*/


    return 0;
}