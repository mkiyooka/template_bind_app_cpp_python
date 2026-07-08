# C++ CLI application (Python バインディングとは独立にコアエンジンをC++から呼び出す)
add_library(cli_lib STATIC
    cli.cpp
)
target_include_directories(cli_lib PUBLIC ${PROJECT_INCLUDE_DIR})
target_link_libraries(cli_lib PUBLIC cxx_core config_lib CLI11::CLI11 fmt::fmt)

add_executable(cmd main.cpp)
set_target_properties(cmd PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
target_link_libraries(cmd PRIVATE cli_lib)
