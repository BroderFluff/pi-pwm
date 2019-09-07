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
};

int getNumPwmDevices(struct pwm_chip *pwm)
{
    if (!pwm->npwm) {
        static char buf[3];
        int fd = open("/sys/class/pwm/pwmchip0/npwm", O_RDONLY);

        memset(buf, 0, sizeof(char) * 3);
        read(fd, buf, 3);
        pwm->npwm = atoi(buf);
        close(fd); 
    }

    return pwm->npwm;
}

int main(int argc, char *argv[])
{
    struct pwm_chip pwmc;

    int num = getNumPwmDevices(&pwmc);

    printf("There are %d number of pwm channels", num);


    return 0;
} 
