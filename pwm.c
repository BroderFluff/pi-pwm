#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct pwm_chip
{
    const char *devicePath;
    int npwm;

    char buf[256];
};

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

int main(int argc, char *argv[])
{
    struct pwm_chip pwmc;

    int num = PWM_num_channels(&pwmc);

    printf("There are %d number of pwm channels\n", num);

    PWM_export_channel(&pwmc, 0);


    return 0;
} 
