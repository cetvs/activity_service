//
// Created by Sergei Tolkachev on 22.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_SERVICE_H
#define UNTITLED3_EOP_ACTIVITY_SERVICE_H

#include <stdio.h>
#include "../data/model/EOP_history_record.h"

int EOP_activity_service_save(EOP_history_record history_record);

char *EOP_activity_service_get_history_record_list();

int EOP_activity_service_history_record_count();

#endif //UNTITLED3_EOP_ACTIVITY_CONTROLLER_H