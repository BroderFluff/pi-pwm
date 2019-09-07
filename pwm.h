#pragma once
#ifndef PWM_H__
#define PWM_H__

#include <stdint.h>

struct pwm_channel
{
    int dutycycle;
    int period;
    int enabled;
};

struct pwm_chip
{
    char *                  dev;    // sysfs path (ex /sys/class/pwm/pwmchip0/)

    struct pwm_channel *    channels;
    int                     num_channels;   // number of pwm channels for this chip

    char                    buf[256];
};

struct pwm_chip* PWM_get_chip(int index);
int PWM_release_chip(struct pwm_chip* pwmc);

int PWM_num_channels(struct pwm_chip *pwmc);

int PWM_export_channel(struct pwm_chip *pwmc, int channel);
int PWM_unexport_channel(struct pwm_chip *pwmc, int channel);

void PWM_channel_set_dutycycle(struct pwm_chip *pwmc, int channel, int dutycycle);
void PWM_channel_set_period(struct pwm_chip *pwmc, int channel, int period);

void PWM_channel_enable(struct pwm_chip *pwmc, int channel);
void PWM_channel_disable(struct pwm_chip *pwmc, int channel);

#endif // PWM_H__
