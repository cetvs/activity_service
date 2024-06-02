//
// Created by Sergei Tolkachev on 22.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_SERVICE_H
#define UNTITLED3_EOP_ACTIVITY_SERVICE_H

#include <stdio.h>
#include "../data/model/EOP_history_record.h"
#include "../data/model/EOP_activity_delete_data.h"

int EOP_activity_service_save(EOP_history_record history_record);

char *EOP_activity_service_get_history_record_list();

int EOP_activity_service_delete_history_record(EOP_activity_delete_request request);

int EOP_activity_service_update_history_record(EOP_history_record history_record);

int EOP_activity_service_history_record_count();

int EOP_activity_service_init_db();

char *EOP_activity_service_sort_by_error_level();

#endif //UNTITLED3_EOP_ACTIVITY_SERVICE_H