# Configuration for Modbus static library.
# Doesn't implement modbus client only a parser
# which then can be read and presented in the gui.

set(SRC
    modbus.cpp
    datastream.cpp
    dataparser.cpp) 

set(HEADERS
    modbus.h
    datastream.h
    dataparser.h)

add_library(modbus STATIC ${SRC} ${HEADERS})

# Add Windows specific Socket library aka WinSock otherwise
# Posix or Berkleys would be obvious choice or perhaps ASIO
# without the huge Boost dependancy if we don't need anything else.
target_link_libraries(modbus wsock32 WS2_32)
