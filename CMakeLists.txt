cmake_minimum_required(VERSION 3.2)

project(joystick)

find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

# add_definitions(-g)

set(JOYSTICK_SOURCES
  joystick.c
  )

add_library(joystick SHARED ${JOYSTICK_SOURCES})
set_target_properties(joystick PROPERTIES SOVERSION 1)
target_link_libraries(joystick ${CMAKE_DL_LIBS})

set(JOYSTICK_LIBRARY "${joystick_BINARY_DIR}/libjoystick.so")

# Our Examples
add_executable(events-listener examples/events-listener.c)
target_link_libraries(events-listener ${JOYSTICK_LIBRARY} ${CMAKE_DL_LIBS})

#
# Python
#
# include_directories(AFTER ${CMAKE_CURRENT_BINARY_DIR} ${Python_INCLUDE_DIRS})
# set(JOYSTICKPP_SOURCES
#   joystick.cpp
#   )
# add_library(pyjoystick MODULE ${JOYSTICKPP_SOURCES})
# target_link_libraries(pyjoystick PRIVATE pybind11::module PUBLIC ${AMI_LIBRARY})
# set_target_properties(pyjoystick PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
#                                             SUFFIX "${PYTHON_MODULE_EXTENSION}")


