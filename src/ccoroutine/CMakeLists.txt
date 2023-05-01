add_library(ccoroutine ccoroutine.c)

target_include_directories(ccoroutine PUBLIC ${PROJECT_SOURCE_DIR}/vendor
                                             ${CMAKE_CURRENT_SOURCE_DIR})
