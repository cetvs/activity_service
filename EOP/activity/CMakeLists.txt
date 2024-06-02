
add_library(
        EOP_Activity
        controllers/EOP_activity_controller.h
        controllers/EOP_activity_controller.c
        data/dao/EOP_activity_dao.h
        data/dao/EOP_activity_dao.c
        data/model/EOP_action_alert.h
        data/EOP_activity_sql.h
        service/EOP_activity_service.h
        service/EOP_activity_service.c
        data/mapper/EOP_activity_mapper.h
        data/mapper/EOP_activity_mapper.c
)

target_include_directories(EOP_Activity PUBLIC ${CMAKE_SOURCE_DIR})