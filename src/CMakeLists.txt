add_executable(run run.c)

target_link_libraries(run ccoroutine)

add_subdirectory(ccoroutine)
