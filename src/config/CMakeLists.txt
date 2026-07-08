# Config sources
add_library(config_lib STATIC
    config_file_loader.cpp
    config_manager.cpp
)
target_include_directories(config_lib PUBLIC ${PROJECT_INCLUDE_DIR})
target_link_libraries(config_lib PUBLIC
    CLI11::CLI11
    tomlplusplus::tomlplusplus
    nlohmann_json::nlohmann_json
    fkYAML_target
    fmt::fmt
)
