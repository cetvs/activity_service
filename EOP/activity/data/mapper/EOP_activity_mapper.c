//
// Created by Sergei Tolkachev on 22.05.2024.
//

#include "../model/EOP_history_record.h"
#include "../../../../third_party/mongoose/mongoose.h"
#include "../model/EOP_activity_delete_data.h"


static long EOP_activity_userId(struct mg_str json) {
    return mg_json_get_long(json, "$.userId", -1);
}
static long EOP_activity_isErrorLevel(struct mg_str json) {
    return mg_json_get_long(json, "$.isErrorLevel", -1);
}

static char *EOP_activity_description(struct mg_str json) {
    return mg_json_get_str(json, "$.description");
}

static long EOP_activity_timestamp(struct mg_str json) {
    return mg_json_get_long(json, "$.timestamp", -1);
}

static long EOP_activity_get_id(struct mg_str json) {
    return mg_json_get_long(json, "$.id", -1);
}

EOP_activity_delete_request EOP_activity_mapper_to_delete_history_record(struct mg_str json) {
    EOP_activity_delete_request request;
    request.id = EOP_activity_get_id(json);
    return request;
}

EOP_history_record EOP_activity_mapper_to_history_record(struct mg_str json) {
    EOP_history_record history_record;
    history_record.userId = EOP_activity_userId(json);
    history_record.isErrorLevel = EOP_activity_isErrorLevel(json);
    history_record.description = EOP_activity_description(json);
    history_record.timestamp = EOP_activity_timestamp(json);
    printf("userId = %ld\n", history_record.userId);
    printf("isErrorLevel = %ld\n", history_record.isErrorLevel);
    printf("description = %s\n", history_record.description);
    printf("timestamp = %ld\n", history_record.timestamp);
    return history_record;
}

char *EOP_history_record_to_json(EOP_history_record historyRecord) {
    char buf[512];
    snprintf(buf, 512, "{\n"
                       "   \"id\": %ld,\n"
                       "   \"userId\": %ld,\n"
                       "   \"isErrorLevel\": %ld,\n"
                       "   \"description\": %s,\n"
                       "   \"timestamp\": \"%ld\"\n"
                       "}",
             historyRecord.id,
             historyRecord.userId,
             historyRecord.isErrorLevel,
             historyRecord.description,
             historyRecord.timestamp
    );
    return buf;
}