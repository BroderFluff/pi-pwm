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
        const size_t size = sizeof(struct pwm_channel) * pwmc->num_channels;
        pwmc->channels = (struct pwm_channel *) malloc(size);
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

int PWM_export_channel(struct pwm_chip *pwmc, int channel)
{
    const int fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    const int len = sprintf(pwmc->buf, "%d", channel);

    if (write(fd, pwmc->buf, len) < 0) {
        perror("PWM_export_channel error: ");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

void PWM_channel_set_dutycycle(struct pwm_chip *pwmc, int channel, int dutycycle)
{
    assert(pwmc);
    const int num_channels = PWM_num_channels(pwmc);
    pwmc->channels[channel]->dutycycle = dutycycle;
}
