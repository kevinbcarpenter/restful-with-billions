set(TargetName restful)

SET(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${TargetName}
    ${SOURCE_DIR}/restful.cpp
    ${SOURCE_DIR}/api/sale.cpp
    ${SOURCE_DIR}/http/handler.cpp
    ${SOURCE_DIR}/http/listener.cpp

    ${PROJECT_SOURCE_DIR}/include/App.cpp
    ${PROJECT_SOURCE_DIR}/include/Logger.cpp
    ${PROJECT_SOURCE_DIR}/include/StringUtils.cpp
)

target_compile_options(${TargetName} PRIVATE
    "-D_GLIBCXX_USE_CXX11_ABI=0"
    "-DNO_ZMQ_SUPPORT"
)

target_include_directories(${TargetName} PRIVATE
    ${SOURCE_DIR}
    ${SOURCE_DIR}/api
    ${SOURCE_DIR}/http

    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/thirdparty
)

target_link_libraries(${TargetName} PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    dl
)

add_subdirectory(test)

# copy App config file to output
set(AppConfig "${TargetName}.json")

add_custom_command(TARGET ${TargetName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${AppConfig}" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)