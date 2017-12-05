configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

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
set_target_properties (qrcon PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VERSION ${qrcon_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${qrcon_HEADERS}"
)

include (GenerateExportHeader)
generate_export_header(qrcon)

include (GNUInstallDirs)
install (TARGETS qrcon
    EXPORT qrcon
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qrcon
)

configure_file(qrcon.pc.in ${CMAKE_CURRENT_BINARY_DIR}/qrcon.pc @ONLY)
install (FILES
    ${CMAKE_CURRENT_BINARY_DIR}/qrcon.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

install (EXPORT qrcon DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qrcon)
