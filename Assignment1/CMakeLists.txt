project(Assignment1 VERSION 1.0.0)

set(CMAKE_CXX_FLAGS, "-lrt")

include_directories(${PROJECT_SOURCE_DIR}/../common/include)

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

    add_executable(${sensor_process_exe_name} 
                   ${sensor_process_SOURCES})

    target_link_libraries(${sensor_process_exe_name}
                          ${Boost_LIBRARIES})
    target_link_libraries(${sensor_process_exe_name}
                          libcommon) 
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
                      libcommon)

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