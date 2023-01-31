cmake_minimum_required(VERSION 3.15)
project(sqlite_migrations)

add_subdirectory(sqlite)

add_executable(sqlite_migrations main.cpp)

target_link_libraries(sqlite_migrations PRIVATE libsqlite)
target_link_libraries(sqlite_migrations PRIVATE pthread)
