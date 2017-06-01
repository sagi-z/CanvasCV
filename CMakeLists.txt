cmake_minimum_required(VERSION 2.8)

project (canvascv)

find_package(OpenCV 3.1.0 REQUIRED)

add_definitions(-std=c++11 -Werror=return-type)

file(GLOB CanvasCV_INCLUDES
    "*.h"
)

file(GLOB CanvasCV_SRC
    "*.h"
    "*.cpp"
)
file(GLOB CanvasCV_SRC_TST "main.cpp")
list(REMOVE_ITEM CanvasCV_SRC "${CanvasCV_SRC_TST}")

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
add_library(canvascv ${LIB_TYPE} ${CanvasCV_SRC})

option(BUILD_TEST "Create an executable" OFF)
if(BUILD_TEST)
        add_executable(shapes ${CanvasCV_SRC_TST})
        target_link_libraries (shapes canvascv ${OpenCV_LIBS})
endif()

install(TARGETS canvascv
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
	COMPONENT library)

install(FILES ${CanvasCV_INCLUDES} DESTINATION include
	COMPONENT library)

set(CPACK_PACKAGE_NAME "canvascv")
set(CPACK_PACKAGE_VENDOR "theimpossiblecode.com")
set(CPACK_PACKAGE_CONTACT "sagi@theimpossiblecode.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interactive canvas widget for OpenCV 3.X")
set(CPACK_PACKAGE_VERSION "0.9.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "9")
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

