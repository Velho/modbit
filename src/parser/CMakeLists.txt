
set(SRC
    datastream.cpp)

set(HEADERS
    modbus.h
    datastream.h)

add_library(parser STATIC ${SRC} ${HEADERS})
