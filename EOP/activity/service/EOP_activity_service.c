//
// Created by Sergei Tolkachev on 22.05.2024.
//


#include <printf.h>
#include <string.h>
#include <EOP/activity/data/model/EOP_activity_delete_data.h>
#include <EOP/activity/data/model/EOP_history_record.h>
#include <EOP/activity/data/dao/EOP_activity_dao.h>

static int EOP_activity_validate_id(int id) {
    if (id < 1) {
        printf("incorrect id");
        return 1;
    }
    return 0;
}

static int EOP_activity_validate_user_id(long user_id) {
    if (user_id < 1) {
        printf("incorrect user_id");
        return 1;
    }
    return 0;
}

static int EOP_activity_validate_is_error_level(long isErrorLevel) {
    if (isErrorLevel < 1) {
        printf("incorrect isErrorLevel");
        return 1;
    }
    return 0;
}

static int EOP_activity_validate_description(char *description) {
    if (strlen(description) > 1000) {
        printf("very long description");
        return 1;
    }
    return 0;
}

static int EOP_activity_validate_timestamp(long timestamp) {
    if (timestamp < 0) {
        printf("incorrect timestamp");
        return 1;
    }
    return 0;
}

static int EOP_activity_validate_delete_request(EOP_activity_delete_request request) {
    if (EOP_activity_validate_id(request.id) == 1) return 1;
    return 0;
}

static int EOP_activity_insert_validate_history_record(EOP_history_record history_record) {
    if (EOP_activity_validate_user_id(history_record.userId) == 1) return 1;
    if (EOP_activity_validate_is_error_level(history_record.isErrorLevel) == 1) return 1;
    if (EOP_activity_validate_description(history_record.description) == 1) return 1;
    if (EOP_activity_validate_timestamp(history_record.timestamp) == 1) return 1;
    return 0;
}

static int EOP_activity_update_validate_history_record(EOP_history_record history_record) {
    if (EOP_activity_validate_id(history_record.id) == 1) return 1;
    if (EOP_activity_validate_description(history_record.description) == 1) return 1;
    return 0;
}

int EOP_activity_service_delete_history_record(EOP_activity_delete_request request) {
    printf("request.id = %ld", request.id);
    if (EOP_activity_validate_delete_request(request) == 1) return 1;

    int result = EOP_activity_dao_delete_history_record(request.id);
    printf("History record deleted\n");
    return result;
}

int EOP_activity_service_init_db() {
    EOP_activity_create_activity_info_table();
}

char *EOP_activity_service_get_history_record_list() {
    return EOP_activity_dao_get_history_record_list();
}

char *EOP_activity_service_get_history_record_by_userId(int userId) {
    printf("userId =%d\n", userId);
    return EOP_dao_get_history_record_by_userId(userId);
}

int EOP_activity_service_history_record_count() {
    return EOP_activity_dao_history_record_count();
}

int EOP_activity_service_save(EOP_history_record history_record) {
    if (EOP_activity_insert_validate_history_record(history_record) == 1) return 1;
    int result = EOP_activity_dao_save_history_record(history_record);
    return result;
}

int EOP_activity_service_update_history_record(EOP_history_record history_record) {
    if (EOP_activity_update_validate_history_record(history_record) == 1) return 1;
    int result = EOP_activity_dao_update_history_record(history_record);
    return result;
}

char *EOP_activity_service_sort_by_error_level() {
    return EOP_activity_dao_sort_by_error_level();
}