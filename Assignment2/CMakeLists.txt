project(Assignment2 VERSION 1.0.0)



include_directories(${PROJECT_SOURCE_DIR}/../common/include)
set(Assignment2_Libs
    libcore
    libheartbeat
    libsync
    libfaulthandle)

set(sensor_process_exe_name syncedSensorProcess)
set(sensor_process_SOURCES
    src/SyncedSensor.cpp
    src/SyncedSensorProcess.cpp
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
                            ${Assignment2_Libs}) 
endif()

target_include_directories(
    ${sensor_process_exe_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)


set(main_exe_name ${PROJECT_NAME})
set(main_SOURCES
    src/FaultMonitor.cpp
    src/main.cpp
)

add_executable(${main_exe_name} ${main_SOURCES})
target_link_libraries(${main_exe_name}
                      ${Assignment2_Libs})

target_include_directories(
    ${main_exe_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
