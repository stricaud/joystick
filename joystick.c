#include <stdio.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "joystick.h"


// https://www.kernel.org/doc/html/latest/input/joydev/joystick-api.html
#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
#define JS_EVENT_AXIS           0x02    /* joystick moved */
#define JS_EVENT_INIT           0x80    /* initial state of device */

struct js_event {
        uint32_t time;     /* event timestamp in milliseconds */
        int16_t value;    /* value */
        uint8_t type;      /* event type */
        uint8_t number;    /* axis/button number */
};

joystick_event_t _handle_event(struct js_event e, joystick_event_cb event_cb, void *userdata)
{

	joystick_event_t event;

	/* printf("%d|%d|%d\n", e.value, e.type, e.number); */

	event.is_init = 0;
	if (e.type > JS_EVENT_AXIS) {
		// We skip initialization events
		event.is_init = 1;
		return event;
	}

	event.time = e.time;
	event.number = e.number;

	if (e.type  == JS_EVENT_BUTTON) {
		if (!e.value) {
			event.action = JOYSTICK_BUTTON_RELEASED;
		} else {
			event.action = JOYSTICK_BUTTON_PRESSED;
		}
	}

	if (e.type == JS_EVENT_AXIS) {
		if (e.type != 2) {
			fprintf(stderr, "Error, unknown event type %d\n", e.type);
			return event;
		}
		if (e.number == 0) { // Left or Right
			if (e.value < 0) {
				event.action = JOYSTICK_AXIS_LEFT;
			} else if (e.value > 0) {
				event.action = JOYSTICK_AXIS_RIGHT;
			} else {
				event.action = JOYSTICK_AXIS_CENTER;
			}
		}
		if (e.number == 1) { // Up or Down
			if (e.value < 0) {
				event.action = JOYSTICK_AXIS_UP;
			} else if (e.value > 0) {
				event.action = JOYSTICK_AXIS_DOWN;
			} else {
				event.action = JOYSTICK_AXIS_CENTER;
			}
		}
	}

	if (event_cb) {
		event_cb(event, userdata);
	}

	return event;
}

int joystick_foreach_event(const char *dev, joystick_event_cb event_cb, void *userdata)
{
	int fd;
	struct js_event e;

	fd = open(dev, O_RDONLY);
	if (!fd) {
		fprintf(stderr, "Cannot open joystick device:%s! Check permissions.\n", dev);
		return -1;
	}

	while (1) {
		while (read(fd, &e, sizeof(e)) > 0) {
			_handle_event(e, event_cb, userdata);
		}
	}

	close(fd);

	return 0;
}


/* To use joystick_foreach_event_noloop you must first use joystick_device_open and then joystick_device_close */
joystick_event_t joystick_foreach_event_noloop(int fd)
{
	struct js_event e;
	joystick_event_t empty_event;

	while (read(fd, &e, sizeof(e)) > 0) {
		return _handle_event(e, NULL, NULL);
	}

	return empty_event;
}

int joystick_device_open(const char *dev)
{
	int fd;

	fd = open(dev, O_RDONLY);
	if (!fd) {
		fprintf(stderr, "Cannot open joystick device:%s! Check permissions.\n", dev);
		return -1;
	}

	return fd;
}

void joystick_device_close(int fd)
{
	close(fd);
}
