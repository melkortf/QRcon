configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)

find_package (Qt5 5.7 REQUIRED COMPONENTS Core Network)

set (qrcon_SOURCES
    qcvarcommand.cpp
    qrconcommand.cpp
    qrconserverconfig.cpp
    qrconsession.cpp
)

set (qrcon_HEADERS
    qcvarcommand.h
    qrconcommand.h
    qrconserverconfig.h
    qrconsession.h

    qrcon.h
)

add_library (qrcon SHARED ${qrcon_SOURCES} ${qrcon_HEADERS})
target_link_libraries (qrcon Qt5::Core Qt5::Network)
set_target_properties (qrcon PROPERTIES CXX_VISIBILITY_PRESET hidden)

include (GenerateExportHeader)
generate_export_header(qrcon)

install (TARGETS qrcon
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    CONFIGURATIONS Release
)
install (FILES ${qrcon_HEADERS}
    DESTINATION include/qrcon
)
