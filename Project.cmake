set(TargetName server)

SET(CMAKE_CXX_FLAGS_DEBUG "-O -ggdb")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${TargetName}
    src/server.cpp
)

target_compile_options(${TargetName} PRIVATE
    "-D_GLIBCXX_USE_CXX11_ABI=0"
)

target_include_directories(${TargetName} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    /opt/homebrew/opt/openssl@3/include
)

target_link_libraries(${TargetName} PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    /opt/homebrew/opt/openssl@3/lib/libssl.a
    dl
    -static-libstdc++
)