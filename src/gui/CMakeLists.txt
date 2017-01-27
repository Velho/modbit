# GUI makes use of Qt for Data Visualization.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)

# List the sources required to build the GUI.
set(FORMS
    mainwindow.ui)

set(SRC
    app.cpp
    mainwindow.cpp)

set(HEADERS
    mainwindow.h)

# Sources and Headers are automatically grouped in VS, but not
# UI files which are essential for Qt application.
source_group(Forms ${FORMS})

# Currently builds on Windows platform.
add_executable(modbit WIN32 ${SRC} ${HEADERS} ${FORMS})

target_link_libraries(modbit modbus)
target_link_libraries(modbit Qt5::Widgets Qt5::Core)
