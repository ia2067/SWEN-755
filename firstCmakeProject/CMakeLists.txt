project(HelloWorld VERSION 0.2.0)

set(SOURCES
    src/HelloWorldPrinter.cpp 
    src/main.cpp
)

# find boost libs for this specific project
find_package(Boost 1.74.0 COMPONENTS thread chrono)

# install them
if(Boost_FOUND)
    message(STATUS "BOOST FOUND!")
    include_directories(${Boost_INLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()


target_include_directories( 
    ${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
