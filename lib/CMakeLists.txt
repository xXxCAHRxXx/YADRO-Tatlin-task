add_library(tape-lib
    exception/TapeException.cpp
    tape/FileTape.cpp
    tape/DelayedTape.cpp
    tape/factory/DelayedFileTapeFactory.cpp
)

target_include_directories(tape-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})