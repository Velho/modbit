# GUI makes use of Qt for Data Visualization.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5::Widgets Qt5::Core)

set(SRC
    app.cpp
    mainwindow.cpp)

set(HEADERS
    mainwindow.h)

# Currently builds on Windows platform.
if(MSVC)
    add_executable(modbit WIN32 ${SRC} ${HEADERS})
endif()

target_link_libraries(modbit parser)
