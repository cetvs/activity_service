//
// Created by Sergei Tolkachev on 22.05.2024.
//



#include <string.h>
#include <third_party/mongoose/mongoose.h>
#include <EOP/activity/service/EOP_activity_service.h>
#include <EOP/activity/data/mapper/EOP_activity_mapper.h>

static bool EOP_activity_validate_sort_by_error_level(char *response_list) {
    return response_list != "";
}

static bool EOP_activity_validate_history_record_list(char *response_list) {
    return response_list != "";
}

static bool EOP_activity_validate_history_record_count(int count) {
    return count > -1;
}

static bool EOP_activity_is_post(struct mg_str method) {
    return strncmp("POST", method.buf, strlen("POST")) == 0;
}

static bool EOP_activity_is_get(struct mg_str method) {
    return strncmp("GET", method.buf, strlen("GET")) == 0;
}

static bool EOP_activity_is_delete(struct mg_str method) {
    return strncmp("DELETE", method.buf, strlen("DELETE")) == 0;
}

static bool EOP_activity_is_put(struct mg_str method) {
    return strncmp("PUT", method.buf, strlen("PUT")) == 0;
}

static void EOP_activity_error_replay(struct mg_connection *pConnection) {
    mg_http_reply(pConnection, 400, "", "Error");
}

static void EOP_activity_success_200_replay(struct mg_connection *pConnection) {
    mg_http_reply(pConnection, 200, "", "Success");
}

static void EOP_activity_success_201_replay(struct mg_connection *pConnection) {
    mg_http_reply(pConnection, 201, "", "Success");
}

static void EOP_activity_history_row_count(struct mg_connection *pConnection) {
    int count = EOP_activity_service_history_record_count();
    if (EOP_activity_validate_history_record_count(count)) {
        char response_count[20]; // Достаточно места для числа и нулевого символа
        sprintf(response_count, "%d", count);
        mg_http_reply(pConnection, 200, "Content-Type: application/json\r\n", response_count);
    } else {
        EOP_activity_error_replay(pConnection);
        printf("EOP_activity_history_row_count error\n");
    }
}

static void EOP_activity_sort_by_error_level(struct mg_connection *pConnection) {
    char *response = EOP_activity_service_sort_by_error_level();
    if (EOP_activity_validate_sort_by_error_level(response)){
        mg_http_reply(pConnection, 200, "Content-Type: application/json\r\n", response);
    } else {
        EOP_activity_error_replay(pConnection);
        printf("EOP_activity_sort_by_error_level error\n");
    }
}

static void EOP_activity_get_history_record_list(struct mg_connection *pConnection) {
    char *response = EOP_activity_service_get_history_record_list();
    if (EOP_activity_validate_history_record_list(response)) {
        mg_http_reply(pConnection, 200, "Content-Type: application/json\r\n", response);
    } else {
        EOP_activity_error_replay(pConnection);
        printf("EOP_activity_get_history_record_list error\n");
    }
}

static void EOP_activity_handle_create_one(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    if (EOP_activity_service_save(EOP_activity_mapper_to_history_record(pMessage->body)) == 0) {
        printf("EOP_activity_service_save");
        EOP_activity_success_201_replay(pConnection);
    } else {
        EOP_activity_error_replay(pConnection);
        printf("EOP_activity_handle_create_one error\n");
    }
}

static void EOP_activity_handle_delete_history_record(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    if (EOP_activity_service_delete_history_record(EOP_activity_mapper_to_delete_history_record(pMessage->body)) == 0) {
        EOP_activity_success_200_replay(pConnection);
    } else {
        EOP_activity_error_replay(pConnection);
        printf("EOP_activity_handle_delete_history_record error\n");
    }
}

static void EOP_activity_handle_update(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    if (EOP_activity_service_update_history_record(EOP_activity_mapper_to_history_record(pMessage->body)) == 0) {
        EOP_activity_success_200_replay(pConnection);
    } else {
        EOP_activity_error_replay(pConnection);
        printf("EOP_activity_handle_update error\n");
    }
}

static void EOP_activity_handle_activity(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    // POST create row
    if (mg_match(pMessage->uri, mg_str("/api/activity/one"), NULL) && EOP_activity_is_post(pMessage->method)) {
        EOP_activity_handle_create_one(pConnection, pMessage);
        return;
    }

    // GET history_record count
    if (mg_match(pMessage->uri, mg_str("/api/activity/history_record_count"), NULL) &&
        EOP_activity_is_get(pMessage->method)) {
        EOP_activity_history_row_count(pConnection);
        return;
    }

    // GET history_record list
    if (mg_match(pMessage->uri, mg_str("/api/activity/history_record_list"), NULL) &&
        EOP_activity_is_get(pMessage->method)) {
        EOP_activity_get_history_record_list(pConnection);
        return;
    }

    // GET sort by critical level
    if (mg_match(pMessage->uri, mg_str("/api/activity/sort_by_error_level"), NULL) &&
        EOP_activity_is_get(pMessage->method)) {
        EOP_activity_sort_by_error_level(pConnection);
        return;
    }

    // DELETE delete history_record
    if (mg_match(pMessage->uri, mg_str("/api/activity/history_record_delete"), NULL) && EOP_activity_is_delete(pMessage->method)) {
        EOP_activity_handle_delete_history_record(pConnection, pMessage);
        return;
    }

    // PUT update history_record
    if (mg_match(pMessage->uri, mg_str("/api/activity/history_record_update"), NULL) &&
            EOP_activity_is_put(pMessage->method)) {
        EOP_activity_handle_update(pConnection, pMessage);
        return;
    }
}

static void EOP_http_matcher(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    if (mg_match(pMessage->uri, mg_str("/api/activity#"), NULL)) {
        EOP_activity_handle_activity(pConnection, pMessage);
    } else {
        struct mg_http_serve_opts opts = {.root_dir = "."};     // For all other URLs,
        mg_http_serve_dir(pConnection, pMessage, &opts);    // Serve static files
    }
}

static void EOP_activity_handle_controller(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
        printf("EOP_activity_handle_controller\n");
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;// Parsed HTTP request
        EOP_http_matcher(c, hm);
    }
}

int EOP_activity_controller_start() {
    EOP_activity_service_init_db();

    setbuf(stdout, 0);
    struct mg_mgr mgr;  // Mongoose event manager. Holds all connections
    mg_mgr_init(&mgr);  // Initialise event manager
    mg_http_listen(&mgr, "http://0.0.0.0:8010", EOP_activity_handle_controller, NULL);  // Setup listener
    for (;;) {
        mg_mgr_poll(&mgr, 1000);  // Infinite event loop
    }
    return 0;
}