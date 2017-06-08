cmake_minimum_required(VERSION 2.8)

project (canvascv)

find_package(OpenCV 3.1.0 REQUIRED)

add_definitions(-std=c++11 -Werror=return-type)
SET( CMAKE_CXX_STANDARD_REQUIRED ON )
SET(CMAKE_CXX_STANDARD 11)

# add include directories to the project
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/examples
)

FILE(GLOB_RECURSE LIB_HEADERS
    "src/*.h"
)

FILE(GLOB_RECURSE LIB_SRCS
    "src/*.h"
    "src/*.cpp"
)

FILE(GLOB_RECURSE EXAMPLE_SRCS
    "examples/*.h"
    "examples/*.cpp"
)

# Allow the developer to select if Dynamic or Static libraries are built
option(BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
# Set the LIB_TYPE variable to STATIC
set(LIB_TYPE STATIC)
if(NOT WIN32)
    if(BUILD_SHARED_LIBS)
            # User wants to build Shared Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
            set(LIB_TYPE SHARED)
            set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/deb/postinst;${CMAKE_CURRENT_SOURCE_DIR}/deb/postrm;")
    endif()
endif()
add_library(canvascv ${LIB_TYPE} ${LIB_SRCS})

option(BUILD_EXAMPLES "Create example executables" OFF)
if(BUILD_EXAMPLES)
    MESSAGE ("Build examples is on, building these examples:")
    FOREACH ( file ${EXAMPLE_SRCS} )
        GET_FILENAME_COMPONENT( target_name ${file} NAME_WE )
        MESSAGE ( "Example '${target_name}' will be created")
        add_executable(${target_name} ${file})
        target_link_libraries (${target_name} canvascv ${OpenCV_LIBS})
    ENDFOREACH()
endif()

install(TARGETS canvascv
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
	COMPONENT library)

FOREACH ( file ${LIB_HEADERS} )
    GET_FILENAME_COMPONENT( dir ${file} DIRECTORY )
    STRING(REPLACE "${CMAKE_SOURCE_DIR}/src" ${PROJECT_NAME} subdir ${dir})
#    MESSAGE ( "${file} will be installed in include/${subdir}")
    INSTALL( FILES ${file} DESTINATION include/${subdir} )
ENDFOREACH()

set(CPACK_PACKAGE_NAME "canvascv")
set(CPACK_PACKAGE_VENDOR "theimpossiblecode.com")
set(CPACK_PACKAGE_CONTACT "sagi@theimpossiblecode.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interactive canvas widget for OpenCV 3.X")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one SET of four (4) backlasshes.
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} CanvasCV")
  set(CPACK_NSIS_MODIFY_PATH ON)
else()
  set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
endif()

# This must always be last!
include(CPack)

