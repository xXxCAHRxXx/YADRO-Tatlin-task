cmake_minimum_required(VERSION 3.12)

project(
    Tatlin-task
    VERSION 1.0
    DESCRIPTION "Tatlin test task"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)

FetchContent_MakeAvailable(nlohmann_json CLI11)

add_subdirectory(bin)
add_subdirectory(lib)


enable_testing()
add_subdirectory(tests)