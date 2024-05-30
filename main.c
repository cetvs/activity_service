#include "EOP/activity/data/dto/activity_dto.h"
#include "third_party/mongoose/mongoose.h"
#include "EOP/activity/controllers/activity_controller.h"

int main(void) {
//    drop_activity_info_table();
    create_activity_info_table();

    AlertAction alertAction = {0, "Открытие каталога", "Пользователь"};

    insert_alert_action(alertAction);
    select_alert_action();

    struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
    mg_mgr_init(&mgr);  // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8100", activity_cotroller_post_activity_info, NULL);  // Setup listener
    for (;;) {
        mg_mgr_poll(&mgr, 1000);  // Infinite event loop
    }
    return 0;
}
