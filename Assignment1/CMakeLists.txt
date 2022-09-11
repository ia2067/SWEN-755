project(Assignment1 VERSION 0.1.0)

set(main_exe_name ${PROJECT_NAME})
set(main_SOURCES
    src/main.cpp
)


add_executable(${main_exe_name} ${main_SOURCES})