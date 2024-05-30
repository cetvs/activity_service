#include "third_party/mongoose/mongoose.h"
#include "EOP/activity/controllers/EOP_activity_controller.h"
#include "EOP/activity/data/dao/EOP_activity_dao.h"

int main(void) {
//    drop_activity_info_table();
//    create_activity_info_table();
//
//    EOP_action_alert alertAction = {0, "Открытие каталога", "Пользователь"};
//
//    insert_alert_action(alertAction);
//    select_alert_action();

    struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
    mg_mgr_init(&mgr);  // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8100", handleController, NULL);  // Setup listener
    for (;;) {
        mg_mgr_poll(&mgr, 1000);  // Infinite event loop
    }
    return 0;
}
