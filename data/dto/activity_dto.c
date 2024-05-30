//
// Created by Sergei Tolkachev on 22.05.2024.
//

#include <sqlite3.h>
#include <stdio.h>
#include "../activity_info.h"
#include "AlertAction.h"

int openDatabase(sqlite3 *db) {
    if (sqlite3_open("mydatabase.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Ошибка открытия базы данных: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
}

int create_table_request(char *sql) {
    sqlite3 *db;
    char *err;

    // Открытие базы данных
    int rc = sqlite3_open("mydatabase.db", &db);
    if (rc) {
        fprintf(stderr, "Не удалось открыть базу данных: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Выполнение запроса
    rc = sqlite3_exec(db, sql, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Не удалось выполнить запрос: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    // Закрытие базы данных
    sqlite3_close(db);
}

int create_activity_info_table() {
    const char *historyRecord = "CREATE TABLE IF NOT EXISTS HistoryRecord ("
                                "id INTEGER PRIMARY KEY,"
                                "userId Text,"
                                "actionAlertId INTEGER,"
                                "actionErrorId INTEGER,"
                                "description: TEXT,"
                                "time TEXT"
                                ")";
    create_table_request(historyRecord);

    const char *action = "CREATE TABLE IF NOT EXISTS AlertAction ("
                         "id INTEGER PRIMARY KEY,"
                         "name TEXT,"
                         "description TEXT"
                         ")";
    create_table_request(action);

    const char *actionType = "CREATE TABLE IF NOT EXISTS ErrorAction ("
                             "id INTEGER PRIMARY KEY,"
                             "name TEXT,"
                             "description TEXT"
                             "criticalLevel INT"
                             ")";
    create_table_request(actionType);
}

int drop_table(char *tableName) {
    printf("drop_table: %s", tableName);
    sqlite3 *db;
    char *err;

    // Открыть базу данных
    openDatabase(db);

    char sql_request[200];
    snprintf(sql_request,
             sizeof(sql_request),
             "DROP TABLE IF EXISTS %s",
             tableName
    );
    printf("Строка: %s\n", sql_request);

    if (sqlite3_exec(db, sql_request, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка вставки данных: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    // Закрыть базу данных
    sqlite3_close(db);

    return 0;
}

int drop_activity_info_table() {
    char *historyRecord = "HistoryRecord";
    drop_table(historyRecord);

    char *errorAlert = "ErrorAction";
    drop_table(errorAlert);

    char *actionError = "AlertAction";
    drop_table(actionError);

//    char *errorAlert = "ErrorAction";
//    drop_table(errorAlert);
//
//    char *actionError = "AlertAction";
//    drop_table(actionError);
}

int execute_query(char *sql_query) {
    sqlite3 *db;
    char *err;
    sqlite3_stmt *stmt;

    openDatabase(db);

    // Подготовка запроса
    if (sqlite3_prepare_v2(db, sql_query, -1, &stmt, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    // Выполнение запроса
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Получение значения из столбца
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *) sqlite3_column_text(stmt, 1);

        // Вывод значений
        printf("ID: %d, Имя: %s\n", id, name);
    }

    // Закрытие подготовленного запроса
    sqlite3_finalize(stmt);

    // Закрытие базы данных
    sqlite3_close(db);

    return 0;
}

int insert_alert_action(AlertAction alertAction) {
    printf("insert_action_type actionType name: %s\n", alertAction.name);

    sqlite3 *db;
    char *err;

    // Открыть базу данных
    openDatabase(db);

    char insertSql[200];
    snprintf(insertSql,
             sizeof(insertSql),
             "INSERT INTO AlertAction (name, description) VALUES ('%s', %s)",
             alertAction.name,
             alertAction.description
    );
    printf("Строка: %s\n", insertSql);

    if (sqlite3_exec(db, insertSql, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка вставки данных: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    // Закрыть базу данных
    sqlite3_close(db);

    return 0;
}

int select_alert_action() {
    sqlite3 *db;

    // 1. Открыть базу данных
    openDatabase(db);

    char *err;
    char *sql = "SELECT * FROM AlertAction";
    sqlite3_stmt *stmt;

    // 2. Подготовить запрос
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    // 3. Выполнить запрос
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // 4. Получить значения из столбцов
        int id = sqlite3_column_int(stmt, 0);
        char *name = (const char *) sqlite3_column_text(stmt, 1);
        char *description = (const char *) sqlite3_column_text(stmt, 2);

        // 5. Вывести значения
        printf("ID: %d, Name: %s, description: %d\n", id, name, description);
    }

    // 6. Закрыть подготовленный запрос
    sqlite3_finalize(stmt);

    // 7. Закрыть базу данных
    sqlite3_close(db);

    return 0;
}