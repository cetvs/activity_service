//
// Created by Sergei Tolkachev on 22.05.2024.
//

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include "../model/EOP_action_alert.h"
#include "../EOP_activity_sql.h"
#include "../model/EOP_history_record.h"
#include "../mapper/EOP_activity_mapper.h"

void add_to_insert_history_record_userId(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 1, history_record.userId); // userId
}

void add_to_insert_history_record_alertActionId(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 2, history_record.alertActionId); // userId
}

void add_to_insert_history_record_errorActionId(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 3, history_record.errorActionId); // errorActionId
}

void add_to_insert_history_record_description(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_text(stmt, 4, history_record.description, -1, SQLITE_TRANSIENT); // description
}

void add_to_insert_history_record_timestamp(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 5, history_record.timestamp); // timestamp
}

void add_to_insert_history_record(sqlite3_stmt *stmt, EOP_history_record history_record) {
    add_to_insert_history_record_userId(stmt, history_record);
    add_to_insert_history_record_alertActionId(stmt, history_record);
    add_to_insert_history_record_errorActionId(stmt, history_record);
    add_to_insert_history_record_description(stmt, history_record);
    add_to_insert_history_record_timestamp(stmt, history_record);
}

char *get_current_dp_Name() {
    return "activity_v11.db";
}

int open_database(sqlite3 **db) {
    if (sqlite3_open(get_current_dp_Name(), db) != SQLITE_OK) {
        fprintf(stderr, "Ошибка открытия базы данных: %s\n", sqlite3_errmsg(db));
        sqlite3_close(*db);
        return 1;
    }
    return 0;
}

int create_table_request(char *sql) {
    sqlite3 *db = NULL;
    char *err;

    // Открытие базы данных

    open_database(&db);

    if (sqlite3_exec(db, sql, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка создания таблицы: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

int create_activity_info_table() {
    create_table_request(EOP_activity_history_record_table_create);
//    create_table_request(EOP_activity_alert_action_table_create);
//    create_table_request(EOP_activity_error_action_table_create);
}

int drop_table(char *tableName) {
    printf("drop_table: %s", tableName);
    sqlite3 *db;
    char *err;

    // Открыть базу данных
    open_database(&db); //передавать ссылку

    char sql_request[200];
    snprintf(sql_request,
             sizeof(sql_request),
             EOP_activity_drop_table_by_name,
             tableName
    );

    if (sqlite3_exec(db, "", NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка drop_table: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    // Закрыть базу данных
    sqlite3_close(db);

    return 0;
}

int drop_activity_info_table() {
    sqlite3 *db;
    char *err;

    char *historyRecord = "HistoryRecord";
    drop_table(historyRecord);

    char *errorAlert = "ErrorAction";
    drop_table(errorAlert);

    char *actionError = "AlertAction";
    drop_table(actionError);
}

int execute_query(char *sql_query) {
    sqlite3 *db;
    char *err;
    sqlite3_stmt *stmt;

    open_database(&db);

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

int insert_alert_action(EOP_action_alert alertAction) {
    printf("insert_action_type actionType name: %s\n", alertAction.name);

    sqlite3 *db;
    char *err;

    // Открыть базу данных
    open_database(&db);

    char insertSql[200];
    snprintf(insertSql,
             sizeof(insertSql),
             "INSERT INTO EOP_action_alert (name, description) VALUES ('%s', %s)",
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

int EOP_activity_dao_history_record_count() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int count = 0;

    open_database(&db);

    // Подготовить запрос
    if (sqlite3_prepare_v2(db, EOP_activity_count_history_record, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Выполнить запрос
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    // Очистить запрос
    sqlite3_finalize(stmt);

    // Закрыть базу данных
    sqlite3_close(db);

    return count;
}

int select_history_record() {
    sqlite3 *db;

    // 1. Открыть базу данных
    open_database(&db);

    char *err;
    char *sql = "SELECT COUNT(*) FROM HistoryRecord";
    sqlite3_stmt *stmt;

    // 2. Подготовить запрос
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса select_history_record: %s\n", err);
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

char *EOP_activity_dao_get_history_record_list() {
    sqlite3 *db;
    char *err;
    sqlite3_stmt *stmt;

    open_database(&db);

    // Подготовить запрос SELECT
    if (sqlite3_prepare_v2(db, EOP_activity_history_record_select_all, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    char *response = "[\n";

    int rc = sqlite3_step(stmt);
    // Выполнить запрос и обработать результат
    while (rc) {
        EOP_history_record history_record;
        // Извлечь данные из каждого столбца
        history_record.id = sqlite3_column_int(stmt, 0);
        history_record.userId = sqlite3_column_int(stmt, 1);
        history_record.alertActionId = sqlite3_column_int(stmt, 2);
        history_record.errorActionId = sqlite3_column_int(stmt, 3);
        history_record.description = (char *) sqlite3_column_text(stmt, 4);
        history_record.timestamp = sqlite3_column_int(stmt, 5);

        // Вывести полученные данные
        printf("id: %s\n", history_record.description);
        rc = sqlite3_step(stmt);
        strcat(response, EOP_history_record_to_json(history_record));
        if (rc == SQLITE_ROW) {
            strcat(response, ",\n");
        }
    }
    strcat(response, "\n]");

    // Очистить запрос
    sqlite3_finalize(stmt);

    // Закрыть базу данных
    sqlite3_close(db);
    printf(response);

    return response;
}

int EOP_activity_dao_save_history_record(EOP_history_record history_record) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    open_database(&db);

    // Подготовить запрос на вставку
    if (sqlite3_prepare_v2(db, EOP_activity_insert_history_record, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    // Вставить данные
    add_to_insert_history_record(stmt, history_record);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Ошибка вставки данных: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    // Очистить запрос
    sqlite3_finalize(stmt);

    // Закрыть базу данных
    sqlite3_close(db);

    printf("Данные успешно добавлены в таблицу.\n");
    return 0;
}

int EOP_activity_dao_delete_history_record(long id) {
    sqlite3 *db;

    int rc = open_database(&db);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db,EOP_activity_delete_history_record,-1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        printf("SQL error");
        sqlite3_free(0);
        sqlite3_close(db);
        return 1;
    }
    return 0;
}