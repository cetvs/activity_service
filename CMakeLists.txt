cmake_minimum_required(VERSION 3.28)
project(untitled3 C)

set(CMAKE_C_STANDARD 17)

set(AppName ${CMAKE_PROJECT_NAME})

find_package(SQLite3 REQUIRED)

add_executable(
        ${AppName} main.c
        third_party/mongoose/mongoose.c
        third_party/mongoose/mongoose.h
        third_party/sqlite/sqlite3.c
        third_party/sqlite/sqlite3.h
)

add_subdirectory(EOP/activity)
target_link_libraries(${AppName} PRIVATE EOP_Activity)