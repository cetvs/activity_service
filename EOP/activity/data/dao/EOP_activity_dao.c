//
// Created by Sergei Tolkachev on 22.05.2024.
//

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <EOP/activity/data/model/EOP_action_alert.h>
#include <EOP/activity/data/EOP_activity_sql.h>
#include <EOP/activity/data/model/EOP_history_record.h>
#include <EOP/activity/data/mapper/EOP_activity_mapper.h>

static void add_to_insert_history_record_userId(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 1, history_record.userId); // userId
}

static void add_to_insert_history_record_isErrorLevel(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 2, history_record.isErrorLevel); // errorActionId
}

static void add_to_insert_history_record_description(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_text(stmt, 3, history_record.description, -1, SQLITE_TRANSIENT); // description
}

static void add_to_insert_history_record_timestamp(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 4, history_record.timestamp); // timestamp
}

static void add_to_insert_history_record_id(sqlite3_stmt *stmt, EOP_history_record history_record) {
    sqlite3_bind_int(stmt, 5, history_record.userId); // userId
}

static void add_to_insert_history_record(sqlite3_stmt *stmt, EOP_history_record history_record) {
    add_to_insert_history_record_userId(stmt, history_record);
    add_to_insert_history_record_isErrorLevel(stmt, history_record);
    add_to_insert_history_record_description(stmt, history_record);
    add_to_insert_history_record_timestamp(stmt, history_record);
}

static void add_to_update_history_record(sqlite3_stmt *stmt, EOP_history_record history_record) {
    add_to_insert_history_record_userId(stmt, history_record);
    add_to_insert_history_record_isErrorLevel(stmt, history_record);
    add_to_insert_history_record_description(stmt, history_record);
    add_to_insert_history_record_timestamp(stmt, history_record);
    add_to_insert_history_record_id(stmt, history_record);
}

static char *get_current_dp_name() {
    return "activity_v13.db";
}

static int EOP_activity_open_database(sqlite3 **db) {
    if (sqlite3_open(get_current_dp_name(), db) != SQLITE_OK) {
        fprintf(stderr, "Ошибка открытия базы данных: %s\n", sqlite3_errmsg(db));
        sqlite3_close(*db);
        return 1;
    }
    return 0;
}

int EOP_activity_create_table_request(char *sql) {
    sqlite3 *db = NULL;
    char *err;

    // Открытие базы данных

    EOP_activity_open_database(&db);

    if (sqlite3_exec(db, sql, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка создания таблицы: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

int EOP_activity_create_activity_info_table() {
    EOP_activity_create_table_request(EOP_activity_history_record_table_create);
}

int EOP_activity_drop_table(char *tableName) {
    printf("EOP_activity_drop_table: %s", tableName);
    sqlite3 *db;
    char *err;

    // Открыть базу данных
    EOP_activity_open_database(&db); //передавать ссылку

    char sql_request[200];
    snprintf(sql_request,
             sizeof(sql_request),
             EOP_activity_drop_table_by_name,
             tableName
    );

    if (sqlite3_exec(db, "", NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка EOP_activity_drop_table: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return 1;
    }

    // Закрыть базу данных
    sqlite3_close(db);

    return 0;
}

int EOP_activity_drop_activity_info_table() {
    sqlite3 *db;
    char *err;

    char *historyRecord = "HistoryRecord";
    EOP_activity_drop_table(historyRecord);

    char *errorAlert = "ErrorAction";
    EOP_activity_drop_table(errorAlert);

    char *actionError = "AlertAction";
    EOP_activity_drop_table(actionError);
}

static int EOP_activity_execute_query(char *sql_query) {
    sqlite3 *db;
    char *err;
    sqlite3_stmt *stmt;

    EOP_activity_open_database(&db);

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

int EOP_activity_insert_alert_action(EOP_action_alert alertAction) {
    printf("insert_action_type actionType name: %s\n", alertAction.name);

    sqlite3 *db;
    char *err;

    // Открыть базу данных
    EOP_activity_open_database(&db);

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

    EOP_activity_open_database(&db);

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

int EOP_activity_select_history_record() {
    sqlite3 *db;

    // 1. Открыть базу данных
    EOP_activity_open_database(&db);

    char *err;
    char *sql = "SELECT COUNT(*) FROM HistoryRecord";
    sqlite3_stmt *stmt;

    // 2. Подготовить запрос
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &err) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса EOP_activity_select_history_record: %s\n", err);
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

    EOP_activity_open_database(&db);

    // Выполнение запроса
    if (sqlite3_prepare_v2(db, EOP_activity_history_record_select_all, -1, &stmt, &err)) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return "";
    }

    char *json = malloc(1024); // Выделяем память для JSON-строки
    if (json == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return NULL;
    }

    sprintf(json, "[\n");
    // Вывод результатов
    int rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        printf("id: %lld\n", sqlite3_column_int64(stmt, 0));
        printf("userId: %lld\n", sqlite3_column_int64(stmt, 1));
        printf("isErrorLevel: %lld\n", sqlite3_column_int64(stmt, 2));
        printf("description: %s\n", sqlite3_column_text(stmt, 3));
        printf("timestamp: %lld\n", sqlite3_column_int64(stmt, 4));
        printf("-----\n");

        EOP_history_record history_record;

        history_record.id = sqlite3_column_int(stmt, 0);
        history_record.userId = sqlite3_column_int(stmt, 1);
        history_record.isErrorLevel = sqlite3_column_int(stmt, 2);
        history_record.description = (char *) sqlite3_column_text(stmt, 3);
        history_record.timestamp = sqlite3_column_int(stmt, 4);

        strcat(json, EOP_history_record_to_json(history_record));

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            strcat(json, ",\n");
        }
    }
    strcat(json, "\n]");
    printf("json = %s", json);

    // Освобождение ресурсов
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return json;
}

int EOP_activity_dao_save_history_record(EOP_history_record history_record) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    EOP_activity_open_database(&db);

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

    int rc = EOP_activity_open_database(&db);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, EOP_activity_delete_history_record, -1, &stmt, 0);
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

int EOP_activity_dao_update_history_record(EOP_history_record history_record) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    int rc = EOP_activity_open_database(&db);

    // Подготовить запрос на вставку
    if (sqlite3_prepare_v2(db, EOP_activity_update_history_record, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки изменения запроса: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    // Вставить данные
    add_to_update_history_record(stmt, history_record);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Ошибка изменения данных: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    // Очистить запрос
    sqlite3_finalize(stmt);

    // Закрыть базу данных
    sqlite3_close(db);

    printf("Данные успешно изменены\n");
    return 0;
}

char *EOP_activity_dao_sort_by_error_level() {
    printf("EOP_activity_dao_sort_by_error_level\n");
    sqlite3 *db;
    char *err;
    sqlite3_stmt *stmt;

    EOP_activity_open_database(&db);

    // Подготовка SQL-запроса

    // Выполнение запроса
    if (sqlite3_prepare_v2(db, EOP_activity_sort_by_isErrorLevel_history_record, -1, &stmt, &err)) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", err);
        sqlite3_free(err);
        sqlite3_close(db);
        return "";
    }

    char *json = malloc(1024); // Выделяем память для JSON-строки
    if (json == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return NULL;
    }

    sprintf(json, "[\n");
    // Вывод результатов
    int rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        EOP_history_record history_record;

        // Вставить данные
        add_to_update_history_record(stmt, history_record);

        strcat(json, EOP_history_record_to_json(history_record));

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            strcat(json, ",\n");
        }
    }
    strcat(json, "\n]");

    // Освобождение ресурсов
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return json;
}