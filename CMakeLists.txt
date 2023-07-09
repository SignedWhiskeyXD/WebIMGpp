cmake_minimum_required(VERSION 3.25)
project(WebIMG)

set(CMAKE_CXX_STANDARD 20)
set(ENABLE_UNIT_TEST ON)

find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)
if(${ENABLE_UNIT_TEST})
    find_package(Catch2 CONFIG REQUIRED)
endif()

include_directories(${Boost_INCLUDE_DIR})
include_directories(./include)

file(GLOB SRCs src/*.cpp)
file(GLOB HEADERs include/*.h)
file(GLOB TESTs test/*.cpp)

add_definitions(-D_WIN32_WINNT=0x0A00)

add_executable(WebIMG main.cpp ${SRCs} ${HEADERs})
target_link_libraries(WebIMG PRIVATE spdlog::spdlog)

if(MSVC)
    target_compile_options(WebIMG PRIVATE /wd4819)
endif ()

if(${ENABLE_UNIT_TEST})
    foreach (testCase IN LISTS TESTs)
        get_filename_component(testCaseName ${testCase} NAME_WE)
        add_executable(${testCaseName} ${testCase} ${SRCs} ${HEADERs})
        target_link_libraries(${testCaseName} PRIVATE
                Catch2::Catch2 Catch2::Catch2WithMain
                spdlog::spdlog)
    endforeach ()
endif ()