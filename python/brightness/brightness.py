#!/usr/bin/env python
# coding: utf-8

import sys
import os


def set_brightness(backlight, value):
    return open('/sys/class/backlight/{backlight}/brightness'
                .format(backlight=backlight), 'wt').write(str(value))


def get_brightness(backlight):
    return int(open('/sys/class/backlight/{backlight}/brightness'
                    .format(backlight=backlight), 'rt').read())


def get_max_brightness(backlight):
    return int(open('/sys/class/backlight/{backlight}/max_brightness'
                    .format(backlight=backlight), 'rt').read())


def less(backlight):
    brightness = get_brightness(backlight)
    max_brightness = get_max_brightness(backlight)

    set_brightness(backlight, max(0, brightness - max(1, max_brightness / 10)))


def more(backlight):
    brightness = get_brightness(backlight)
    max_brightness = get_max_brightness(backlight)

    set_brightness(backlight, max(0, brightness + max(1, max_brightness / 10)))


def get_device_list():
    return os.listdir('/sys/class/backlight/')


def show_help():
    print('''brightness.py (less|more) device

Devices:
    {}'''.format('\n    '.join(get_device_list())))


if __name__ == '__main__':
    if len(sys.argv) == 3 and sys.argv[1] in ('less', 'more'):
        if sys.argv[2] not in get_device_list():
            print('Device {} not found'.format(sys.argv[2]))
        elif sys.argv[1] == 'less':
            less(sys.argv[2])
        elif sys.argv[1] == 'more':
            more(sys.argv[2])

    else:
        show_help()
