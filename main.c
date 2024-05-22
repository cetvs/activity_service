#include <stdio.h>
#include <sqlite3.h>

#include "mongoose/mongoose.h"
#include "controllers/activity_controller.h"
#include "data/dto/activity_dto.h"

static int register_sql() {
    sqlite3 *db;
    char *err;
    const char *sql = "CREATE TABLE IF NOT EXISTS ActivityInfo (id INTEGER PRIMARY KEY, userId Integer, name TEXT, age INTEGER)";

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

int main(void) {
    register_sql();
    create_activity_info_table();

    struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
    mg_mgr_init(&mgr);  // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8100", activity_cotroller_post_activity_info , NULL);  // Setup listener
    for (;;) {
        mg_mgr_poll(&mgr, 1000);  // Infinite event loop
    }
}
