project (QRcon)
cmake_minimum_required (VERSION 3.7 FATAL_ERROR)

if (UNIX AND NOT APPLE)
    set (LINUX 1)
endif ()

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# set configuration types (msvc/xcode)
if (CMAKE_CONFIGURATION_TYPES)
    set (CMAKE_CONFIGURATION_TYPES Debug Release)
    set (CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we actually need" FORCE
)
endif()

# set CMAKE_BUILD_TYPE (makefiles)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set (CMAKE_BUILD_TYPE Debug)
else ()
    set (CMAKE_BUILD_TYPE Release)
endif ()

# set CXX_FLAGS
if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    set (CMAKE_CXX_FLAGS_RELEASE "-DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT -DNDEBUG")
    set (CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall -Wextra -Wpedantic")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined -Wl,--exclude-libs,ALL")
    else ()
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-undefined,error")
    endif ()
elseif (MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set (CMAKE_CXX_FLAGS_DEBUG "/W4")
    set (CMAKE_CXX_FLAGS_RELEASE "/DQT_NO_DEBUG /DQT_NO_DEBUG_OUTPUT")
endif ()

set (CMAKE_CXX_STANDARD "14")

# find version
if (NOT qrcon_VERSION)
    file (READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION qrcon_VERSION)
    string (REPLACE "\n" "" qrcon_VERSION ${qrcon_VERSION})
    
    include(GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_SHA1)
    
    if (NOT "${GIT_SHA1}" STREQUAL "GITDIR-NOTFOUND")
        # shorten the sha
        string (SUBSTRING "${GIT_SHA1}" 0 10 GIT_HASH)
        set (qrcon_VERSION ${qrcon_VERSION}-${GIT_HASH}-git)
    endif ()
endif ()

set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set (CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory (src)

# uninstall target
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target (
    uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)
