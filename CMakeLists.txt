cmake_minimum_required(VERSION 3.12)

project(
    Tatlin-task
    VERSION 1.0
    DESCRIPTION "Tatlin test task"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(bin)
# add_subdirectory(lib)


# enable_testing()
# add_subdirectory(tests)