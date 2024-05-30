//
// Created by Sergei Tolkachev on 22.05.2024.
//

#ifndef UNTITLED3_ACTIVITY_DTO_H
#define UNTITLED3_ACTIVITY_DTO_H

#include "../activity_info.h"
#include "AlertAction.h"

int create_activity_info_table();

int create_table_request(char *sql);

int drop_activity_info_table();

int drop_table(char *tableName);

int insert_alert_action(AlertAction alertAction);

int select_alert_action();

#endif //UNTITLED3_ACTIVITY_DTO_H
