#ifndef _DATA_CONFIG_PODS_H_
#define _DATA_CONFIG_PODS_H_

#include "csv/money.h"

struct owner {
    int id;
    char *name;
};

struct account {
    int id;
    char *name;
    int owner;
    money_t balance;
};

struct channel {
    int id;
    char *name;
};

#endif /* _DATA_CONFIG_PODS_H_ */
