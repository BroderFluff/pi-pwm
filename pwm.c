#include "pwm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

void PWM_export_channel(struct pwm_chip *pwmc, int channel)
{
    int fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);

    int len = sprintf(pwmc->buf, "%d", channel);
    write(fd, pwmc->buf, len);
}
