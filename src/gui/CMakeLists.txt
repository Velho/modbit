# GUI makes use of Qt for Data Visualization.

find_package(Qt5Widgets)
find_package(Qt5Charts)

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
target_link_libraries(modbit Qt5::Charts Qt5::Widgets Qt5::Core)
