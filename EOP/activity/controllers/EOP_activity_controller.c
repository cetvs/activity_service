//
// Created by Sergei Tolkachev on 22.05.2024.
//

#include "../../../third_party/mongoose/mongoose.h"
#include "../service/EOP_activity_service.h"
#include "../data/mapper/EOP_activity_mapper.h"

static bool EOP_activity_is_post(struct mg_str method) {
    return strncmp("POST", method.buf, strlen("POST")) == 0;
}

static bool EOP_activity_is_get(struct mg_str method) {
    return strncmp("GET", method.buf, strlen("GET")) == 0;
}

static void EOP_activity_error_replay(struct mg_connection *pConnection) {
    mg_http_reply(pConnection, 400, "", "Error");
}

static void EOP_activity_success_201_replay(struct mg_connection *pConnection) {
    mg_http_reply(pConnection, 201, "", "Success");
}

static void EOP_activity_handle_create_one(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    if (EOP_activity_service_save(EOP_activity_mapper_to_history_record(pMessage->body)) == 0) {
        printf("EOP_activity_service_save");
        EOP_activity_success_201_replay(pConnection);
    } else {
        EOP_activity_error_replay(pConnection);
    }
}

static void EOP_activity_handle_create_all_activity(struct mg_connection *pConnection, struct mg_http_message *pMessage) {

}

void EOP_activity_handle_activity(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    // POST create row
    if (mg_match(pMessage->uri, mg_str("/api/activity/one"), NULL) && EOP_activity_is_post(pMessage->method)) {
        EOP_activity_handle_create_one(pConnection, pMessage);
        return;
    }

    // GET all activity
    if (mg_match(pMessage->uri, mg_str("/api/activity/all"), NULL) && EOP_activity_is_get(pMessage->method)) {
        EOP_activity_handle_create_all_activity(pConnection, pMessage);
        return;
    }
}

void http_matcher(struct mg_connection *pConnection, struct mg_http_message *pMessage) {
    if (mg_match(pMessage->uri, mg_str("/api/activity#"), NULL)) {
        EOP_activity_handle_activity(pConnection, pMessage);
    } else {
        struct mg_http_serve_opts opts = {.root_dir = "."};     // For all other URLs,
        mg_http_serve_dir(pConnection, pMessage, &opts);    // Serve static files
    }
}

void handle_controller(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
        printf("handle_controller\n");
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;// Parsed HTTP request
        http_matcher(c, hm);
    }
}

//void test(struct mg_connection *c, int ev, void *ev_data) {
//    if (ev == MG_EV_HTTP_MSG) {
//        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
//        bool is_match = mg_match(hm->uri, mg_str("/api/sum"), NULL);
//        if (is_match) {
//            char *str = mg_json_get_str(hm->body, "$[0]");  // Extract first
//            activity_info data = {str};
//            printf("value of a_static: %s\n", data.name);
//            mg_http_reply(
//                    c,
//                    200,
//                    "Content-Type: application/json\r\n",
//                    "{%m:%g}\n", MG_ESC("result"),
//                    str
//            );
//        }
//    }
//}