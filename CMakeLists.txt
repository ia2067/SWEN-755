cmake_minimum_required(VERSION 3.5.0)

project(SWEN755 VERSION 1.0.0)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lrt -pthread")

if(CMAKE_CXX_COMPILER MATCHES "win32$" AND
   CMAKE_C_COMPILER MATCHES "win32$")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/windows.cmake")
else() 
    # linux
endif()

project(SWEN-755)


message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "Is crosscompiling: ${CMAKE_CROSSCOMPILING}")
message(STATUS "Build System: ${CMAKE_HOST_SYSTEM}")
message(STATUS "Target System: ${CMAKE_SYSTEM}")

# Define C++14
set(CMAKE_CXX_STANDARD 14)

#enabled testing
enable_testing()

# set(Boost_DEBUG 1)
# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED OFF)
# set(Boost_USE_STATIC_RUNTIME OFF)

add_subdirectory(common)
add_subdirectory(firstCmakeProject)
add_subdirectory(Assignment1)
add_subdirectory(Assignment2)
add_subdirectory(Assignment3)
add_subdirectory(Assignment4)


option(BUILD_DOC "Build Documentation" ON)

find_package(Doxygen
             REQUIRED dot)

if(Doxygen_FOUND)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen.in)
    set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    configure_file(${DOXYFILE_IN} 
                   ${DOXYFILE} @ONLY)

    message("Doxygen build started.")
    message(${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_target(docs
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
                      COMMENT "Generating API documentation With Doxygen"
                      VERBATIM)
endif(Doxygen_FOUND)