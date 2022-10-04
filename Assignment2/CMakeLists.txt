project(Assignment2 VERSION 1.0.0)



include_directories(${PROJECT_SOURCE_DIR}/../common/include)
set(Assignment1_Libs
    libcore
    libheartbeat
    libsync)


set(main_exe_name ${PROJECT_NAME})
set(main_SOURCES
    src/main.cpp
)

add_executable(${main_exe_name} ${main_SOURCES})
target_link_libraries(${main_exe_name}
                      ${Assignment1_Libs})

target_include_directories(
    ${main_exe_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
