cmake_minimum_required(VERSION 3.22)
project(WebIMG)

set(CMAKE_CXX_STANDARD 20)
set(ENABLE_UNIT_TEST ON)

find_package(Boost REQUIRED)
find_package(spdlog REQUIRED)
find_package(openssl REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(./include .)

file(GLOB SRCs src/*.cpp)
file(GLOB HEADERs include/*.h)
file(GLOB SERVLETs Servlet/*.cpp Servlet/*.h)

set(WebIMGFiles ${SRCs} ${HEADERs} ${SERVLETs})

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

add_executable(WebIMG main.cpp ${WebIMGFiles})
target_link_libraries(WebIMG PRIVATE spdlog::spdlog OpenSSL::SSL OpenSSL::Crypto)

if(MSVC)
    target_compile_options(WebIMG PRIVATE /wd4819)
endif ()

if(${ENABLE_UNIT_TEST})
    add_subdirectory(test)
endif ()