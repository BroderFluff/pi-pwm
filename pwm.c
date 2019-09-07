#include "pwm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static const size_t PWM_PATH_LEN = 32;

struct pwm_channel
{
    struct pwm_chip *   pwmc;
    int                 index;
    char                path[128];

    int                 dutycycle;
    int                 period;
    int                 enabled;
};

struct pwm_chip
{
    char *                  dev;    // sysfs path (ex /sys/class/pwm/pwmchip0/)

    struct pwm_channel **   channels;
    int                     num_channels;   // number of pwm channels for this chip

    char                    buf[256];
};

static int get_pwm_chip_path(int index, char *buf)
{
    return sprintf(buf, "/sys/class/pwm/pwmchip%d", index);
}

static void init_channels(struct pwm_chip *pwmc)
{
    strcat(strncpy(pwmc->buf, pwmc->dev, strlen(pwmc->dev)), "/npwm");

    const int fd = open(pwmc->buf, O_RDONLY);
    assert(fd > 0);
    
    memset(pwmc->buf, 0, sizeof(char) * 3);
    read(fd, pwmc->buf, 3);    
    pwmc->num_channels = atoi(pwmc->buf);

    if (pwmc->num_channels > 0) {
        const size_t size = sizeof(struct pwm_channel *) * pwmc->num_channels;
        pwmc->channels = (struct pwm_channel **) malloc(size);
    }

    close(fd); 
}

struct pwm_chip* PWM_get_chip(int index)
{
    struct pwm_chip *pwmc = (struct pwm_chip *) malloc(sizeof (struct pwm_chip));
    if (!pwmc) {
        return NULL;
    }

    memset(pwmc, 0, sizeof (pwmc));

    // Create pwm chips system path
    pwmc->dev = (char *) malloc(sizeof (char) * PWM_PATH_LEN);
    get_pwm_chip_path(index, pwmc->dev);

    // Check pwm chips system path exists
    if (access(pwmc->dev, F_OK) < 0) {
        fprintf(stderr, "%s does not exist!", pwmc->dev);
        PWM_release_chip(pwmc);
        return NULL;
    }

    init_channels(pwmc);

    return pwmc;
}

int PWM_release_chip(struct pwm_chip *pwmc)
{
    if (pwmc) {
        if (pwmc->dev) {
            free(pwmc->dev);
        }
        free(pwmc);
    }

    return 0;
}

int PWM_num_channels(struct pwm_chip *pwmc)
{
    assert(pwmc);
    return pwmc->num_channels;
}

struct pwm_channel* PWM_export_channel(struct pwm_chip *pwmc, int channel_index)
{
    assert(pwmc);
    assert(pwmc->channels);
    assert(pwmc->num_channels > channel_index);

    memset(pwmc->buf, 0, sizeof(char) * 256);
    strcat(strncpy(pwmc->buf, pwmc->dev, strlen(pwmc->dev)), "/export");

    puts(pwmc->buf);
    
    // Write channel index to export
    const int fd = open(pwmc->buf, O_WRONLY);
    assert(fd > 0);
    const int len = sprintf(pwmc->buf, "%d", channel_index);
    if (write(fd, pwmc->buf, len) < 0) {
        perror("PWM_export_channel error: ");
        close(fd);
        return NULL;
    }
    close(fd);

    struct pwm_channel *ch = pwmc->channels[channel_index] = (struct pwm_channel *) malloc(sizeof(struct pwm_channel));
    memset(ch, 0, sizeof (struct pwm_channel));

    ch->pwmc = pwmc;

    sprintf(pwmc->buf, "/pwm%d", channel_index);
    strcat(strcpy(ch->path, pwmc->dev), pwmc->buf);

    ch->index = channel_index;

    return ch;
}

int PWM_unexport_channel(struct pwm_channel *ch)
{
    assert(ch);
    assert(ch->pwmc);
    assert(ch == ch->pwmc->channels[ch->index]);

    struct pwm_chip *pwmc = ch->pwmc;
    pwmc->channels[ch->index] = NULL;

    free(ch);

    return 0;
}

static void param_write(struct pwm_channel *ch, int value, const char *name)
{
    struct pwm_chip *pwmc = ch->pwmc;

    strcat(strcpy(pwmc->buf, ch->path), name);
    const int fd = open(pwmc->buf, O_WRONLY);

    const size_t len = sprintf(pwmc->buf, "%d", ch->dutycycle);
    write(fd, pwmc->buf, len);
    close(fd);
}

void PWM_update_channel(struct pwm_channel *ch, const struct pwm_params *params, const uint32_t mask)
{
    assert(ch);
    assert(ch->pwmc);
    assert(ch == ch->pwmc->channels[ch->index]);

    struct pwm_chip *pwmc = ch->pwmc;

    if (!mask) {
        return;
    }

    if ((mask & PWM_DUTYCYCLE_BIT) == PWM_DUTYCYCLE_BIT) {
        ch->dutycycle = params->dutycycle;

        param_write(ch, ch->dutycycle, "/duty_cycle");
    }

    if (mask & PWM_PERIOD_BIT) {
        ch->period = params->period;
        param_write(ch, ch->period, "/period");
    }

    if (mask & PWM_ENABLE_BIT) {
        ch->enabled = params->enable;
        param_write(ch, ch->enabled, "/enable");
    }
}



void PWM_channel_set_dutycycle(struct pwm_channel *ch, int dutycycle)
{
    assert(ch);

    ch->dutycycle = dutycycle;
}

void PWM_channel_set_period(struct pwm_channel *ch, int period)
{
    assert(ch);

    ch->period = period;
}

void PWM_channel_enable(struct pwm_channel *ch)
{
    assert(ch);

    ch->enabled = 1;    
}

void PWM_channel_disable(struct pwm_channel *ch)
{
    assert(ch);
    assert(ch->pwmc);

    ch->enabled = 0;
}
