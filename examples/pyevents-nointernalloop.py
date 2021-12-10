#!/usr/bin/env python3
import pyjoystick

js = pyjoystick.Joystick("/dev/input/js0")

while True:
    js_event = js.Read()
    if js.ReceivedData():
        print(js_event.GetAction())
        print(js_event.GetNumber())
