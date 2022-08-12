cmake_minimum_required (VERSION 3.14)
project (example-session-provider)

set (CMAKE_CXX_STANDARD 20)

find_package (Boost REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

add_library (mod SHARED
    src/connection.h
    src/connection.cpp
    src/messages.h
    src/mod.h
    src/mod.cpp
    src/serializer.h
    src/serializer.cpp
    src/session.h
    src/session_provider.h
    src/synchorized_queue.h)

target_link_libraries (mod PUBLIC Boost::boost)
target_link_libraries (mod PUBLIC Threads::Threads)

