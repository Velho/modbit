# GUI makes use of Qt for Data Visualization.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)

set(FORMS
    mainwindow.ui)

set(SRC
    app.cpp
    mainwindow.cpp)

set(HEADERS
    mainwindow.h)

# Currently builds on Windows platform.
add_executable(modbit WIN32 ${SRC} ${HEADERS} ${FORMS})

target_link_libraries(modbit parser)
target_link_libraries(modbit Qt5::Widgets Qt5::Core)
