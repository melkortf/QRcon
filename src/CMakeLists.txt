configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

find_package (Qt5Core REQUIRED)
include_directories (${Qt5Core_INCLUDE_DIRS})

find_package (Qt5Network REQUIRED)
include_directories (${Qt5Network_INCLUDE_DIRS})

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_BINARY_DIR})

set (qrcon_SOURCES
    qrconcommand.cpp
    qrconsession.cpp
)

set (qrcon_HEADERS
    qrconcommand.h
    qrconsession.h

    qrcon.h
    qrconexport.h
)

add_library (qrcon SHARED ${qrcon_SOURCES} ${qrcon_HEADERS})
target_link_libraries (qrcon Qt5::Core Qt5::Network)
set_target_properties (qrcon PROPERTIES CXX_VISIBILITY_PRESET hidden)
install (TARGETS qrcon
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    CONFIGURATIONS Release
)
install (FILES ${qrcon_HEADERS}
    DESTINATION include/qrcon
)
