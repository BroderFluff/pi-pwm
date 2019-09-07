#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    puts("Hej!");

    int fd = open("/sys/class/pwm/pwmchip0/npwm", O_RDONLY);

    char buf[3];
    read(fd, buf, 3);
    puts(buf);

    return 0;
} 
