cmake_minimum_required(VERSION 2.8)

project (canvascv)

find_package(OpenCV 3.1.0 REQUIRED)
SET(CMAKE_GIT_REPO "CanvasCV")

if(NOT WIN32)
    add_definitions(-Werror=return-type)
    set(CXX_11_FS_LIB "-lstdc++fs")
else()
    set(CXX_11_FS_LIB "")
endif()
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 11)

# add include directories to the project
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/examples
    ${OpenCV_INCLUDE_DIRS}
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

if(${OpenCV_VERSION} VERSION_GREATER "3.1.0")
    ADD_DEFINITIONS(-DOPENCV_HAS_WAITKEYEX=1)
else()
    ADD_DEFINITIONS(-DOPENCV_HAS_WAITKEYEX=0)
endif()

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
        target_link_libraries (${target_name} canvascv ${CXX_11_FS_LIB} ${OpenCV_LIBS})
    ENDFOREACH()
endif()

install(TARGETS canvascv
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
	COMPONENT library)

FOREACH ( file ${LIB_HEADERS} )
    GET_FILENAME_COMPONENT( dir ${file} DIRECTORY )
    STRING(REPLACE "${CMAKE_SOURCE_DIR}/src/" "" subdir ${dir})
    #MESSAGE ( "${file} will be installed in include/${subdir}")
    INSTALL( FILES ${file} DESTINATION include/${subdir} )
ENDFOREACH()

set(CPACK_PACKAGE_NAME "canvascv")
set(CPACK_PACKAGE_VENDOR "theimpossiblecode.com")
set(CPACK_PACKAGE_CONTACT "sagi@theimpossiblecode.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interactive canvas with shapes and widget for OpenCV 3.X")
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

option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR
      "Doxygen is needed to build the documentation. Please install it correctly")
  endif()

  FILE(WRITE ${CMAKE_BINARY_DIR}/concat.cmake "
      FILE(READ \${SRC1} S1)
      FILE(READ \${SRC2} S2)
      FILE(WRITE \${DST} \${S1}\${S2})
      ")

  # Allow css changes
  SET(CMAKE_DOXYGEN_EXTRA_CSS "")
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxygen_style.css")
      SET(CMAKE_DOXYGEN_EXTRA_CSS "${CMAKE_CURRENT_SOURCE_DIR}/doxygen_style.css")
  endif()

  # These variables are used in Doxyfile.in (must be defined at this point):
  # CMAKE_DOXYGEN_EXTRA_CSS
  # CMAKE_DOXYGEN_HTML_HEADER
  # CMAKE_GIT_REPO
  # CPACK_PACKAGE_VERSION
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxygen_in/install_linux.md.in ${CMAKE_CURRENT_SOURCE_DIR}/doxygen_in/install_linux.md @ONLY)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxygen_in/install_windows.md.in ${CMAKE_CURRENT_SOURCE_DIR}/doxygen_in/install_windows.md @ONLY)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxygen_header.html.in")
      set(CMAKE_DOXYGEN_HTML_HEADER "${CMAKE_SOURCE_DIR}/doxygen_header.html")
  endif()
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxygen_in/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile @ONLY)

  # Allow html header changes
  if(${CMAKE_DOXYGEN_HTML_HEADER})
      add_custom_target (doc ALL
          COMMAND ${DOXYGEN_EXECUTABLE} -w html ${CMAKE_BINARY_DIR}/doxygen_header_orig.html.in ${CMAKE_BINARY_DIR}/ignore_footer.html ${CMAKE_BINARY_DIR}/ignore_css.css
          COMMAND ${CMAKE_COMMAND} -D SRC1=${CMAKE_CURRENT_SOURCE_DIR}/doxygen_header.html.in
                                       -D SRC2=${CMAKE_BINARY_DIR}/doxygen_header_orig.html.in
                                       -D DST=${CMAKE_SOURCE_DIR}/doxygen_header.html
                                       -P ${CMAKE_BINARY_DIR}/concat.cmake
          COMMAND ${CMAKE_COMMAND} -E remove_directory doc
          COMMAND ${CMAKE_COMMAND} -E make_directory doc
          COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
          )
  else()
      add_custom_target (doc ALL
          COMMAND ${CMAKE_COMMAND} -E remove_directory doc
          COMMAND ${CMAKE_COMMAND} -E make_directory doc
          COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
          )
  endif()
endif()

# This must always be last!
include(CPack)

