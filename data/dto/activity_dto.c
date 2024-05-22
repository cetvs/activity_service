//
// Created by Sergei Tolkachev on 22.05.2024.
//

#include <sqlite3.h>
#include <stdio.h>
#include "../activity_info.h"

int create_activity_info_table(){
    sqlite3 *db;
    char *err;
    const char *sql = "CREATE TABLE IF NOT EXISTS ActivityInfo (id INTEGER PRIMARY KEY, name TEXT, age INTEGER)";

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


int execute_query(const char *db_file, const char *sql) {
    sqlite3 *db;
    int rc;

    // Открыть соединение с базой данных
    rc = sqlite3_open(db_file, &db);
    if (rc) {
        fprintf(stderr, "Не удалось открыть базу данных: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Подготовить запрос
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Не удалось подготовить запрос: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Выполнить запрос
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        fprintf(stderr, "Не удалось выполнить запрос: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Обработать результаты запроса
    // ...

    // Завершить запрос
    sqlite3_finalize(stmt);

    // Закрыть соединение с базой данных
    sqlite3_close(db);

    return 0;
}

//void add_new_record_activity_info(activity_info data) {
//    char *sql = ""
//    execute_query();
//}
