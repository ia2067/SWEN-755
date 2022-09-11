project(HelloWorld VERSION 0.2.0)

set(main_exe_name ${PROJECT_NAME})
set(main_SOURCES
    src/main.cpp
)

set(helper_exe_name ${PROJECT_NAME}_helper)
set(helper_SOURCES
    src/HelloWorldPrinter.cpp 
    src/HelloWorld.cpp
)

set(Boost_NeededLibs
    thread 
    chrono
    program_options
)

# find boost libs for this specific project
find_package(Boost 1.74.0 COMPONENTS  ${Boost_NeededLibs})

# install them
if(Boost_FOUND)
    message(STATUS "BOOST libs for ${PROJECT_NAME} found!")
    include_directories(${Boost_INLUDE_DIRS})
    # Create starting program
    add_executable(${main_exe_name} ${main_SOURCES})
    # add boost libs to it
    target_link_libraries(${main_exe_name} ${Boost_LIBRARIES})

    # Create helper program
    add_executable(${helper_exe_name} ${helper_SOURCES})
    # add boost libs to it too!
    target_link_libraries(${helper_exe_name} ${Boost_LIBRARIES})
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()


target_include_directories( 
    ${helper_exe_name}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include/
)
