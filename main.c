#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>

#define TOUCHDEV "/dev/input/by-id/usb-ELAN_Touchscreen-event-if00"

int touch_start(int fd, int es, struct input_event *ev)
{
    for (int i = 0;  i < es; i++)
    {
        if (ev[i].type == EV_KEY && ev[i].code == 330 && ev[i].value == 1)
        {
            return 1;
        }
    }
    return 0;
}

int touch_y(int fd, int es, struct input_event *ev)
{
    for (int i = 0;  i < es; i++)
    {
        if (ev[i].type == EV_ABS && ev[i].code == 1)
        {
            if (ev[i].value > 1056)
            {
                write(0, "J", 1);
            }
            else if (ev[i].value > 0)
            {
                write(0, "K", 1);
            }
        }
    }
}

int main()
{
    int fd = open(TOUCHDEV, O_RDONLY);

    struct input_event ev[64];
    ssize_t rb;
    size_t es;

    do
    {
        rb = read(fd, ev, sizeof(struct input_event)*64);
        es = rb / sizeof(struct input_event);

        if (touch_start(fd, es, ev))
        {
            touch_y(fd, es, ev);
        }
    }
    while (rb != 0);

    close(fd);
    return 0;
}
