project(Assignment1 VERSION 0.1.0)

set(CMAKE_CXX_FLAGS, "-lrt")
# common library
set(common_lib_name libcommon)
set(common_SOURCES 
    src/common/HeartbeatSender.cpp
    src/common/Thread.cpp
    src/common/HeartbeatMessage.cpp
    src/common/HeartbeatReceiver.cpp
)



set(Boost_CommonLibNeededLibs
    thread 
    chrono
    serialization
)

# find boost libs for this specific project
find_package(Boost 1.71.0 COMPONENTS  ${Boost_CommonLibNeededLibs})

# install them
if(Boost_FOUND)
    message(STATUS "BOOST libs for ${PROJECT_NAME}/${common_lib_name}  found!")
    include_directories(${Boost_INCLUDE_DIRS})
    # Create common library
    add_library(${common_lib_name} 
                STATIC 
                ${common_SOURCES}
    )
    # add boost libs to it
    target_link_libraries(${common_lib_name} ${Boost_LIBRARIES} rt)
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()

target_include_directories(
    ${common_lib_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

set(sensor_process_exe_name sensorProcess)
set(sensor_process_SOURCES
    src/Sensor.cpp
    src/SensorProcess.cpp
)

set(Boost_ProgramLibs
    program_options
)

find_package(Boost 1.71.0 COMPONENTS ${Boost_ProgramLibs})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${sensor_process_exe_name} ${sensor_process_SOURCES})
    target_link_libraries(${sensor_process_exe_name}
                          ${common_lib_name})   
    target_link_libraries(${sensor_process_exe_name}
                          ${Boost_LIBRARIES})
endif()

target_include_directories(
    ${sensor_process_exe_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)



set(main_exe_name ${PROJECT_NAME})
set(main_SOURCES
    src/main.cpp
)

add_executable(${main_exe_name} ${main_SOURCES})
target_link_libraries(${main_exe_name}
                      ${common_lib_name})

target_include_directories(
    ${main_exe_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)


option(BUILD_DOC "Build Documentation" ON)

find_package(Doxygen
             REQUIRED dot)

if(Doxygen_FOUND)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Assignment1.doxygen)
    set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    configure_file(${DOXYFILE_IN} 
                   ${DOXYFILE} @ONLY)

    message("Doxygen build started.")
    message(${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_target(doc
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
                      COMMENT "Generating API documentation With Doxygen"
                      VERBATIM)
endif(Doxygen_FOUND)