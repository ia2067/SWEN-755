project(TestCommonLibs VERSION 1.0.0)

include_directories(${PROJECT_SOURCE_DIR}/../include)

include(BoostTestHelpers.cmake)

add_boost_test(heartbeat/testHeartbeat.cpp
               libheartbeat)

add_boost_test(sync/testSync.cpp
               libsync)