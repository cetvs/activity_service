//
// Created by Sergei Tolkachev on 22.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_DAO_H
#define UNTITLED3_EOP_ACTIVITY_DAO_H

#include "../model/EOP_action_alert.h"
#include "../model/EOP_activity_err_code.h"
#include "../model/EOP_history_record.h"

int create_activity_info_table();

int create_table_request(char *sql);

int drop_activity_info_table();

int drop_table(char *tableName);

int insert_alert_action(EOP_action_alert alertAction);

int select_history_record();

int select_table_row_count();

int EOP_activity_dao_save_history_record(EOP_history_record history_record);

#endif //UNTITLED3_EOP_ACTIVITY_DAO_H
