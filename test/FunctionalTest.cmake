set(FunctionaTestTargetName ${TargetName}_functional_test)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${FunctionaTestTargetName}
    src/client.cpp

    ${SOURCE_DIR}/detail/creditcard.cpp
)

target_include_directories(${FunctionaTestTargetName} PRIVATE
    ${SOURCE_DIR}
    ${SOURCE_DIR}/api
    ${SOURCE_DIR}/detail
    ${SOURCE_DIR}/http

    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/thirdparty
)

target_link_libraries(${FunctionaTestTargetName} PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
)