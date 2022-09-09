cmake_minimum_required(VERSION 3.5.0)
project(HelloWorld VERSION 0.2.0)

set(SOURCES
    src/HelloWorldPrinter.cpp 
    src/main.cpp
)


add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories( 
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
