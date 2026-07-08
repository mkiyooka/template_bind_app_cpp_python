# Core C++ library
add_library(cxx_core STATIC
    commands/commit.cpp
    commands/fetch.cpp
    commands/log.cpp
)

target_include_directories(cxx_core PUBLIC
    ${PROJECT_INCLUDE_DIR}
)
