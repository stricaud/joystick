#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum _joystick_event_action_t {
	JOYSTICK_UNKNOWN,
	JOYSTICK_AXIS_UP,
	JOYSTICK_AXIS_DOWN,
	JOYSTICK_AXIS_LEFT,
	JOYSTICK_AXIS_RIGHT,
	JOYSTICK_AXIS_CENTER,
	JOYSTICK_BUTTON_PRESSED,
	JOYSTICK_BUTTON_RELEASED,
};
typedef enum _joystick_event_action_t joystick_event_action_t;

static const char *joystick_event_action_name[] = {
	"JOYSTICK_UNKNOWN",
	"JOYSTICK_AXIS_UP",
	"JOYSTICK_AXIS_DOWN",
	"JOYSTICK_AXIS_LEFT",
	"JOYSTICK_AXIS_RIGHT",
	"JOYSTICK_AXIS_CENTER",
	"JOYSTICK_BUTTON_PRESSED",
	"JOYSTICK_BUTTON_RELEASED",
};

struct _joystick_event_t {
	uint32_t time;
	joystick_event_action_t action;
	uint8_t number;
	int is_init;
};
typedef struct _joystick_event_t joystick_event_t;

typedef int (*joystick_event_cb)(joystick_event_t event, void *userdata);

int joystick_foreach_event(const char *dev, joystick_event_cb event_cb, void *userdata);

joystick_event_t joystick_foreach_event_noloop(int fd);
int joystick_device_open(const char *dev);
void joystick_device_close(int fd);

#ifdef __cplusplus
}
#endif

#endif // _JOYSTICK_H_
