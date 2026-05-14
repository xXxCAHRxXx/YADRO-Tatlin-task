add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    tape-lib
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})