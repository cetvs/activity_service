//
// Created by Sergei Tolkachev on 30.05.2024.
//

#ifndef UNTITLED3_EOP_ACTIVITY_TABLES_H
#define UNTITLED3_EOP_ACTIVITY_TABLES_H

char *EOP_activity_history_record_table_create = "CREATE TABLE IF NOT EXISTS HistoryRecord ("
                                                 "id INTEGER PRIMARY KEY,"
                                                 "userId INTEGER,"
                                                 "alertActionId INTEGER,"
                                                 "errorActionId INTEGER,"
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
#endif //UNTITLED3_EOP_ACTIVITY_TABLES_H
