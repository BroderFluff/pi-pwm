#pragma once
#ifndef PWM_H__
#define PWM_H__

#include <stdint.h>

#define PWM_DUTYCYCLE_BIT (1U << 0)
#define PWM_PERIOD_BIT (1U << 1)
#define PWM_POLARITY_BIT (1U << 2)
#define PWM_ENABLE_BIT (1U << 3)

struct pwm_channel;
struct pwm_params
{
    int duty_cycle;
    int period;
    int polarity;
    int enable;
    uint32_t mask;
};
struct pwm_chip;

struct pwm_chip* PWM_get_chip(int index);
int PWM_release_chip(struct pwm_chip* pwmc);

int PWM_num_channels(struct pwm_chip *pwmc);

struct pwm_channel * PWM_export_channel(struct pwm_chip *pwmc, int channel_index);
int PWM_unexport_channel(struct pwm_channel *ch);
void PWM_update_channel(struct pwm_channel *ch, const struct pwm_params *params, uint32_t mask);

void PWM_channel_set_dutycycle(struct pwm_channel *ch, int dutycycle);
void PWM_channel_set_period(struct pwm_channel *ch, int period);

void PWM_channel_enable(struct pwm_channel *ch);
void PWM_channel_disable(struct pwm_channel *ch);

#endif // PWM_H__
