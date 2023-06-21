#ifndef _DATA_CONFIG_PODS_H_
#define _DATA_CONFIG_PODS_H_

#include "csv/money.h"

struct owner {
    int id;
    char *name;
};

struct account_type {
    int id;
    char *name;
};

struct account {
    int id;
    char *name;
    int owner;
    int type;
    char *desc;
    money_t balance;
    char *bill_config;
};

struct batch {
    int id;
    char *title;
};

#endif /* _DATA_CONFIG_PODS_H_ */
