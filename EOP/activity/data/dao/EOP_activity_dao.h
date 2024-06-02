//
// Created by Sergei Tolkachev on 22.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_DAO_H
#define UNTITLED3_EOP_ACTIVITY_DAO_H

#include "../model/EOP_action_alert.h"
#include "../model/EOP_activity_err_code.h"
#include "../model/EOP_history_record.h"

int EOP_activity_create_activity_info_table();

int create_table_request(char *sql);

int EOP_activity_drop_activity_info_table();

int EOP_activity_drop_table(char *tableName);

int insert_alert_action(EOP_action_alert alertAction);

int EOP_activity_select_history_record();

char *EOP_activity_dao_get_history_record_list();

int EOP_activity_dao_history_record_count();

int EOP_activity_dao_save_history_record(EOP_history_record history_record);

int EOP_activity_dao_delete_history_record(long id);

int EOP_activity_dao_update_history_record(EOP_history_record history_record);

#endif //UNTITLED3_EOP_ACTIVITY_DAO_H
