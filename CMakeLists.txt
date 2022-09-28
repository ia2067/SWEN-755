cmake_minimum_required(VERSION 3.5.0)

project(SWEN755 VERSION 1.0.0)

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

# Define C++11
set(CMAKE_CXX_STANDARD 11)

# set(Boost_DEBUG 1)
# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED OFF)
# set(Boost_USE_STATIC_RUNTIME OFF)

add_subdirectory(common)
add_subdirectory(firstCmakeProject)
add_subdirectory(Assignment1)
