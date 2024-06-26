//
// Created by Sergei Tolkachev on 22.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_MAPPER_H
#define UNTITLED3_EOP_ACTIVITY_MAPPER_H

#include <stdio.h>
#include "../model/EOP_history_record.h"
#include "../model/EOP_activity_delete_data.h"

EOP_history_record EOP_activity_mapper_to_history_record(struct mg_str json);

EOP_activity_delete_request EOP_activity_mapper_to_delete_history_record(struct mg_str json);

char *EOP_history_record_to_json(EOP_history_record historyRecord);

long EOP_activity_mapper_userId(struct mg_str json);

#endif //UNTITLED3_EOP_ACTIVITY_CONTROLLER_H