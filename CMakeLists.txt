cmake_minimum_required(VERSION 3.28)
project(untitled3 C)

set(CMAKE_C_STANDARD 17)

find_package(SQLite3 REQUIRED)

add_executable(
        untitled3 main.c
        third_party/mongoose/mongoose.c
        third_party/mongoose/mongoose.h
        EOP/activity/controllers/activity_controller.h
        EOP/activity/data/activity_info.h
        EOP/activity/controllers/activity_controller.c
        EOP/activity/data/dto/activity_dto.h
        EOP/activity/data/dto/activity_dto.c
        EOP/activity/data/dto/AlertAction.h
)

include_directories(${SQLite3_INCLUDE_DIR})
target_link_libraries(untitled3 ${SQLite3_LIBRARIES})
