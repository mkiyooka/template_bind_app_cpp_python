# C++ tests
add_executable(test_commit
    test_commit.cpp
)
target_link_libraries(test_commit PRIVATE cxx_core)
add_test(NAME test_commit COMMAND test_commit)

add_executable(test_fetch
    test_fetch.cpp
)
target_link_libraries(test_fetch PRIVATE cxx_core)
add_test(NAME test_fetch COMMAND test_fetch)

add_executable(test_log
    test_log.cpp
)
target_link_libraries(test_log PRIVATE cxx_core)
add_test(NAME test_log COMMAND test_log)
