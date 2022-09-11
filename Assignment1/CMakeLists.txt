project(Assignment1 VERSION 0.1.0)

# common library
set(common_lib_name libcommon)
set(common_SOURCES 
    src/common/HeartbeatSender.cpp
    src/common/Thread.cpp
)



set(Boost_CommonLibNeededLibs
    thread 
    chrono
)

# find boost libs for this specific project
find_package(Boost 1.74.0 COMPONENTS  ${Boost_CommonLibNeededLibs})

# install them
if(Boost_FOUND)
    message(STATUS "BOOST libs for ${PROJECT_NAME}/${common_lib_name}  found!")
    include_directories(${Boost_INLUDE_DIRS})
    # Create common library
    add_library(${common_lib_name} 
                STATIC 
                ${common_SOURCES}
    )
    # add boost libs to it
    target_link_libraries(${common_lib_name} ${Boost_LIBRARIES})
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()

target_include_directories(
    ${common_lib_name}
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