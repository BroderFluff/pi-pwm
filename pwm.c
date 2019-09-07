#include "pwm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static const size_t PWM_MAX_LEN = 32;

static int get_pwm_chip_path(int index, char *buf)
{
    return sprintf(buf, "/sys/class/pwm/pwmchip%d", index);
}

struct pwm_chip* PWM_get_chip(int index)
{
    struct pwm_chip *pwmc = (struct pwm_chip *) malloc(sizeof (struct pwm_chip));
    if (!pwmc) {
        return NULL;
    }

    pwmc->dev = (char *) malloc(sizeof (char) * PWM_MAX_LEN);
    get_pwm_chip_path(index, pwmc->dev);

    if (access(pwmc->dev, F_OK) < 0) {
        fprintf(stderr, "%s does not exist!", pwmc->dev);

        free(pwmc->dev);
        free(pwmc);
        
        return NULL;
    }



    return 
}

int PWM_num_channels(struct pwm_chip *pwmc)
{
    if (!pwmc->npwm) {
        int fd = open("/sys/class/pwm/pwmchip0/npwm", O_RDONLY);

        memset(pwmc->buf, 0, sizeof(char) * 3);
        read(fd, pwmc->buf, 3);
        pwmc->npwm = atoi(pwmc->buf);
        close(fd); 
    }

    return pwmc->npwm;
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

void PWM_set_dutycycle(struct pwm_chip *pwmc, int channel, int dutycycle)
{

}
