#include <iostream>

#include "pybind11/pybind11.h"
#include <pybind11/functional.h>

#include "joystick.h"

namespace py = pybind11;

int fd = 0; // This is a global to avoid using a class for the joystickpp_foreach_event. 

class JoystickEvent {
public:
  uint32_t GetTime(void) { return _time; };
  joystick_event_action_t GetAction(void) { return _action; };
  uint8_t GetNumber(void) { return _number; };
  void SetTime(uint32_t time) { _time = time; };
  void SetAction(joystick_event_action_t action) { _action = action; };
  void SetNumber(uint8_t number) { _number = number; };
  joystick_event_t c_event_get() { return _c_joystick_event; };
  void c_event_set(joystick_event_t c_event) { _c_joystick_event = c_event; };
private:
  uint32_t _time;
  joystick_event_action_t _action;
  uint8_t _number;

  joystick_event_t _c_joystick_event;
};

int joystickpp_foreach_event(const char *dev, std::function<void(JoystickEvent &)> event_cb)
{

  if (!fd) {
    fd = joystick_device_open(dev);
  }
  
  JoystickEvent jsevent;
  
  while (1) {
    joystick_event_t ev;
    ev = joystick_foreach_event_noloop(fd);
    if (!ev.is_init) {
      jsevent.SetTime(ev.time);
      jsevent.SetAction(ev.action);
      jsevent.SetNumber(ev.number);
    
      event_cb(jsevent);
    }
  }
  
  joystick_device_close(fd);
  
  return 0;
}

class Joystick {
public:
  Joystick(const std::string &dev);
  ~Joystick(void);
  JoystickEvent read(void);
  int received_data(void) { return _received_data; };
private:
  int _fd;
  int _received_data;
};

Joystick::Joystick(const std::string &dev)
{
  _fd = joystick_device_open(dev.c_str());
  _received_data = 0;
}

Joystick::~Joystick(void)
{
  joystick_device_close(fd);
}

JoystickEvent Joystick::read(void)
{
  JoystickEvent jsevent;
  joystick_event_t ev;

  _received_data = 0;
  jsevent.SetAction(JOYSTICK_UNKNOWN);

  ev = joystick_foreach_event_noloop(_fd);
  if (ev.action == JOYSTICK_UNKNOWN) {
    return jsevent;
  }
  
  if (!ev.is_init) {
    _received_data = 1;
    jsevent.SetTime(ev.time);
    jsevent.SetAction(ev.action);
    jsevent.SetNumber(ev.number);
    return jsevent;
  }

  return jsevent;
}

PYBIND11_MODULE(pyjoystick, m) {
  py::enum_<joystick_event_action_t>(m, "JoystickAction")
    .value("JOYSTICK_UNKNOWN", joystick_event_action_t::JOYSTICK_UNKNOWN)
    .value("JOYSTICK_AXIS_UP", joystick_event_action_t::JOYSTICK_AXIS_UP)
    .value("JOYSTICK_AXIS_DOWN", joystick_event_action_t::JOYSTICK_AXIS_DOWN)
    .value("JOYSTICK_AXIS_LEFT", joystick_event_action_t::JOYSTICK_AXIS_LEFT)
    .value("JOYSTICK_AXIS_RIGHT", joystick_event_action_t::JOYSTICK_AXIS_RIGHT)
    .value("JOYSTICK_AXIS_CENTER", joystick_event_action_t::JOYSTICK_AXIS_CENTER)
    .value("JOYSTICK_BUTTON_PRESSED", joystick_event_action_t::JOYSTICK_BUTTON_PRESSED)
    .value("JOYSTICK_BUTTON_RELEASED", joystick_event_action_t::JOYSTICK_BUTTON_RELEASED)
    .export_values();
  py::class_<JoystickEvent>(m, "JoystickEvent")
    .def(py::init<>())
    .def("GetTime", &JoystickEvent::GetTime)
    .def("GetAction", &JoystickEvent::GetAction)
    .def("GetNumber", &JoystickEvent::GetNumber)
    .def("SetTime", &JoystickEvent::SetTime)
    .def("SetAction", &JoystickEvent::SetAction)
    .def("SetNumber", &JoystickEvent::SetNumber);    
  m.def("foreach_event", &joystickpp_foreach_event);
  py::class_<Joystick>(m, "Joystick")
    .def(py::init<const std::string &>())
    .def("ReceivedData", &Joystick::received_data)
    .def("Read", &Joystick::read);
}
