# file(GLOB common_files ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

set(common_files
    ${CMAKE_CURRENT_LIST_DIR}/src/cameras.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gpio.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ignitor_collection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/sensor_collection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/radio.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/radio_queue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/rocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/status.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/XBee.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/states/armed.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/states/ascent_to_apogee.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/states/drogue_descent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/states/mach_lock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/states/main_descent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/states/pressure_delay.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/states/standby.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/tasks/main_tasks.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tasks/led_blinker.cpp

    ${CMAKE_CURRENT_LIST_DIR}/src/hardware/ignitor.cpp

    ${CMAKE_CURRENT_LIST_DIR}/lib/CalcHelpers/barometric.cpp
)
target_sources(
    TantalusStage1
    PRIVATE
    ${common_files}
)
target_sources(
    TantalusStage2
    PRIVATE
    ${common_files}
)
target_sources(
    Hollyburn
    PRIVATE
    ${common_files}
)
