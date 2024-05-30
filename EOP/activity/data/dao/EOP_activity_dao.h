//
// Created by Sergei Tolkachev on 22.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_DAO_H
#define UNTITLED3_EOP_ACTIVITY_DAO_H

#include "../model/EOP_action_alert.h"

int create_activity_info_table();

int create_table_request(char *sql);

int drop_activity_info_table();

int drop_table(char *tableName);

int insert_alert_action(EOP_action_alert alertAction);

int select_alert_action();

#endif //UNTITLED3_EOP_ACTIVITY_DAO_H
