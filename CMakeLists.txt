cmake_minimum_required(VERSION 3.25)
project(WebIMG)

set(CMAKE_CXX_STANDARD 20)
set(ENABLE_UNIT_TEST ON)

find_package(Boost REQUIRED)
if(${ENABLE_UNIT_TEST})
    find_package(Catch2 CONFIG REQUIRED)
endif()

include_directories(${Boost_INCLUDE_DIR})
include_directories(./include)

file(GLOB SRCs src/*.cpp)
file(GLOB HEADERs include/*.h)
file(GLOB TESTs test/*.cpp)

add_executable(WebIMG main.cpp ${SRCs} ${HEADERs})
if(${ENABLE_UNIT_TEST})
    foreach (testCase IN LISTS TESTs)
        get_filename_component(testCaseName ${testCase} NAME_WE)
        add_executable(${testCaseName} ${testCase} ./include/ThreadingQueue.h)
        target_link_libraries(${testCaseName} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
    endforeach ()
endif ()