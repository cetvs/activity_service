//
// Created by Sergei Tolkachev on 22.05.2024.
//

#include "../../../third_party/mongoose/mongoose.h"
#include "../data/activity_info.h"

void activity_cotroller_post_activity_info(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        bool is_match = mg_match(hm->uri, mg_str("/api/sum"), NULL);
        if (is_match) {
            char *str = mg_json_get_str(hm->body, "$[0]");  // Extract first
            activity_info data = {str};
            printf("value of a_static: %s\n", data.name);
            mg_http_reply(
                    c,
                    200,
                    "Content-Type: application/json\r\n",
                    "{%m:%g}\n", MG_ESC("result"),
                    str
            );
        }
    }
}

