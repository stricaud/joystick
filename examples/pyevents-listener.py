#!/usr/bin/env python3
import pyjoystick

def joystick_event(event):
    print("Got an event")
    print("Action:%s" % str(event.GetAction()))
    print("Number:%s" % str(event.GetNumber()))
    
    
pyjoystick.foreach_event("/dev/input/js0", joystick_event)
