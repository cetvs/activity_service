//
// Created by Sergei Tolkachev on 22.05.2024.
//


#include <printf.h>
#include <string.h>
#include "../data/model/EOP_history_record.h"
#include "../data/dao/EOP_activity_dao.h"

static int EOP_activity_validate_user_id(long user_id) {
    if (user_id < 1) {
        printf("incorrect student_id");
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
        printf("very long description");
        return 1;
    }
    return 0;
}

static int EOP_activity_validate_history_record(EOP_history_record history_record) {
    if (EOP_activity_validate_user_id(history_record.userId) == 1) return 1;
    if (EOP_activity_validate_description(history_record.description) == 1) return 1;
    if (EOP_activity_validate_timestamp(history_record.timestamp) == 1) return 1;
    return 0;
}

int EOP_activity_service_get_all_activity() {
    printf("Visit count calculated\n");
}

int EOP_activity_service_save(EOP_history_record history_record) {
    if (EOP_activity_validate_history_record(history_record) == 1) return 1;
    int result = EOP_activity_dao_save_history_record(history_record);
    printf("description: %s", history_record.description);
    return result;
}