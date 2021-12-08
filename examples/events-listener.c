#include <stdio.h>
#include "../joystick.h"

int joystick_event_handler(joystick_event_t event, void *userdata)
{
	printf("Got an event:\n");
	printf("\ttime: %ld\n", event.time);
	printf("\taction: %s\n", joystick_event_action_name[event.action]);
	printf("\tnumber: %d\n", event.number);

	return 0;
}

int main(int argc, char **argv)
{
	return joystick_foreach_event("/dev/input/js0", joystick_event_handler, NULL);
}
