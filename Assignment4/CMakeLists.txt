project(Assignment4 VERSION 1.0.0)

include_directories(${PROJECT_SOURCE_DIR}/../common/include)

set(main_exe_name ${PROJECT_NAME})
set(main_SOURCES
    src/HelloApplication.cpp
    src/main.cpp
)

set(Boost_ProgramLibs
    system
    thread
    filesystem
    program_options
)

find_package(Boost 1.71.0 COMPONENTS ${Boost_ProgramLibs})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(${main_exe_name} ${main_SOURCES})

    target_link_libraries(${main_exe_name}
                          ${Boost_LIBRARIES}
                          wt
                          wthttp)
endif()



target_include_directories(
    ${main_exe_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)