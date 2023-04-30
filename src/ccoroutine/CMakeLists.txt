add_library(ccoroutine editor.c ui.c)

target_link_libraries(ccoroutine md4c)

target_include_directories(ccoroutine PUBLIC ${PROJECT_SOURCE_DIR}/vendor
                                             ${CMAKE_CURRENT_SOURCE_DIR})
