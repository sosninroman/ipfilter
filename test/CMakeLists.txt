file(GLOB_RECURSE test_SRC
        "./*.cpp"
        )

include_directories(../)

add_executable(test_cli gtest.cpp ${test_SRC} ../ipfilter.cpp ../split.cpp)

target_link_libraries(test_cli gtest)
