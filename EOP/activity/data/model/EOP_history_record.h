//
// Created by Sergei Tolkachev on 29.05.2024.
//

#ifndef UNTITLED3_ACTION_H
#define UNTITLED3_ACTION_H

typedef struct EOP_history_record {
    long id;
    long userId;
    long isErrorLevel;
    char *description;
    long timestamp;
} EOP_history_record;

#endif //UNTITLED3_ACTION_H
