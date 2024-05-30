cmake_minimum_required(VERSION 3.28)
project(untitled3 C)

set(CMAKE_C_STANDARD 17)

find_package(SQLite3 REQUIRED)

add_executable(
        untitled3 main.c
        third_party/mongoose/mongoose.c
        third_party/mongoose/mongoose.h
        EOP/activity/controllers/EOP_activity_controller.h
        EOP/activity/controllers/EOP_activity_controller.c
        EOP/activity/data/dao/EOP_activity_dao.h
        EOP/activity/data/dao/EOP_activity_dao.c
        EOP/activity/data/model/EOP_action_alert.h
        EOP/activity/data/EOP_activity_tables.h
)

include_directories(${SQLite3_INCLUDE_DIR})
target_link_libraries(untitled3 ${SQLite3_LIBRARIES})
