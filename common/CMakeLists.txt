# common library
set(common_lib_name libcommon)
set(common_SOURCES 
    src/HeartbeatSender.cpp
    src/Thread.cpp
    src/HeartbeatMessage.cpp
    src/HeartbeatReceiver.cpp
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

set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

target_include_directories(
    ${common_lib_name}
    PRIVATE
    ${COMMON_INCLUDE_DIR}
)