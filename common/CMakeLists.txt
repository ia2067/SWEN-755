set(COMMON_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

##### START LIB CORE #####
set(core_lib_name libcore)
set(core_SOURCES
    src/core/Thread.cpp
    # src/core/MessageQueue.cpp
    )


set(Boost_CoreNeededLibs
    chrono
    serialization
    )

find_package(Boost 1.71.0 COMPONENTS  ${Boost_CoreNeededLibs})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # Create core library
    add_library(libcore 
                STATIC 
                ${core_SOURCES}
                )
    # add boost libs to it
    target_link_libraries(libcore 
                          ${Boost_LIBRARIES} 
                          rt)
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()

target_include_directories(
    libcore
    PRIVATE
    ${COMMON_INCLUDE_DIR}
)
##### END LIB CORE #####

##### START LIB HEARTBEAT #####
set(heartbeat_lib_name libheartbeat)
set(heartbit_SOURCES
    src/heartbeat/Sender.cpp
    src/heartbeat/Message.cpp
    src/heartbeat/Receiver.cpp
    )

set(Boost_HearbeatNeededLibs
    chrono
    serialization
    )

find_package(Boost 1.71.0 COMPONENTS  ${Boost_HearbeatNeededLibs})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # Create common library
    add_library(libheartbeat 
                STATIC 
                ${heartbit_SOURCES}
    )
    # add boost libs to it
    target_link_libraries(libheartbeat 
                          libcore
                          ${Boost_LIBRARIES} 
                          rt)
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()

target_include_directories(
    libheartbeat
    PRIVATE
    ${COMMON_INCLUDE_DIR}
)
##### END LIB HEARTBEAT #####

#### START LIB SYNC ####
set(sync_lib_name libsync)
set(sync_SOURCES
    src/sync/Sender.cpp
    src/sync/Receiver.cpp
    src/sync/Message.cpp
    )

set(Boost_SyncNeededLibs
    serialization
    )

find_package(Boost 1.71.0 COMPONENTS  ${Boost_SyncNeededLibs})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # Create common library
    add_library(libsync 
                STATIC 
                ${sync_SOURCES}
    )
    # add boost libs to it
    target_link_libraries(libsync 
                          libcore
                          ${Boost_LIBRARIES} 
                          rt)
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()

target_include_directories(
    libsync
    PRIVATE
    ${COMMON_INCLUDE_DIR}
)
#### END LIB SYNC ####


#### START LIB FAULTHANDLE ####
set(faulthandle_lib_name libfaulthandle)
set(faulthandle_SOURCES
    src/faulthandle/Client.cpp
    src/faulthandle/Server.cpp
    src/faulthandle/Message.cpp
    )

set(Boost_SyncNeededLibs
    serialization
    )

find_package(Boost 1.71.0 COMPONENTS  ${Boost_SyncNeededLibs})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # Create common library
    add_library(${faulthandle_lib_name}
                STATIC 
                ${faulthandle_SOURCES}
    )
    # add boost libs to it
    target_link_libraries(${faulthandle_lib_name} 
                          libcore
                          ${Boost_LIBRARIES} 
                          rt)
else()
    message(SEND_ERROR "BOOST NOT FOUND!")
endif()

target_include_directories(
    ${faulthandle_lib_name}
    PRIVATE
    ${COMMON_INCLUDE_DIR}
)
#### END LIB FAULTHANDLE

#### ADD TESTING ####
add_subdirectory(test)