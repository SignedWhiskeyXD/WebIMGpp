find_package(Catch2 CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

if(MSVC)
    target_compile_options(WebIMG PRIVATE /wd4819)
endif ()

add_executable(testMatcher testMatcher.cpp ${WebIMGFiles})
target_link_libraries(testMatcher PRIVATE
        Catch2::Catch2 Catch2::Catch2WithMain
        spdlog::spdlog)

add_executable(testBoostMySQL testBoostMySQL.cpp)
target_link_libraries(testBoostMySQL PRIVATE
        Catch2::Catch2 Catch2::Catch2WithMain
        OpenSSL::SSL OpenSSL::Crypto)

add_executable(testSQLPool testSQLPool.cpp ${WebIMGFiles})
target_link_libraries(testSQLPool PRIVATE
        Catch2::Catch2 Catch2::Catch2WithMain
        OpenSSL::SSL OpenSSL::Crypto
        spdlog::spdlog)