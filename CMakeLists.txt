cmake_minimum_required(VERSION 3.28)
project(untitled3 C)

set(CMAKE_C_STANDARD 17)

find_package(SQLite3 REQUIRED)

add_executable(
        untitled3 main.c
        mongoose/mongoose.c
        mongoose/mongoose.h
        controllers/activity_controller.h
        data/activity_info.h
        controllers/activity_controller.c
        data/dto/activity_dto.h
        data/dto/activity_dto.c
)

include_directories(${SQLite3_INCLUDE_DIR})
target_link_libraries(untitled3 ${SQLite3_LIBRARIES})
