//
// Created by Sergei Tolkachev on 30.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_SQL_H
#define UNTITLED3_EOP_ACTIVITY_SQL_H

char *EOP_activity_database_name = "activity_v13.db";

char *EOP_activity_history_record_table_create = "CREATE TABLE IF NOT EXISTS HistoryRecord ("
                                                 "id INTEGER PRIMARY KEY,"
                                                 "userId INTEGER,"
                                                 "isErrorLevel INTEGER,"
                                                 "description TEXT,"
                                                 "timestamp INTEGER"
                                                 ")";

const char *EOP_activity_alert_action_table_create = "CREATE TABLE IF NOT EXISTS AlertAction ("
                                                     "id INTEGER PRIMARY KEY,"
                                                     "name TEXT,"
                                                     "description TEXT"
                                                     ")";

const char *EOP_activity_error_action_table_create = "CREATE TABLE IF NOT EXISTS ErrorAction ("
                                                     "id INTEGER PRIMARY KEY,"
                                                     "name TEXT,"
                                                     "description TEXT,"
                                                     "criticalLevel INT"
                                                     ")";


const char *EOP_activity_insert_history_record = "INSERT INTO HistoryRecord (userId, isErrorLevel, description, timestamp) VALUES (?, ?, ?, ?)";

const char *EOP_activity_history_record_select_all = "SELECT * FROM HistoryRecord";

const char *EOP_activity_count_history_record = "SELECT COUNT(*) FROM HistoryRecord";

const char *EOP_activity_drop_table_by_name = "DROP TABLE IF EXISTS %s";

const char *EOP_activity_delete_history_record = "DELETE FROM HistoryRecord WHERE id = ?";

const char *EOP_activity_update_history_record = "UPDATE HistoryRecord SET userId = ?, isErrorLevel = ?, description = ?, timestamp = ? WHERE id = ?";
#endif //UNTITLED3_EOP_ACTIVITY_SQL_H
