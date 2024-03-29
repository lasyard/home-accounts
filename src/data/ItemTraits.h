#ifndef _DATA_ITEM_TRAITS_H_
#define _DATA_ITEM_TRAITS_H_

#include "CsvRowTraits.h"
#include "item.h"

template <> class CsvRowTraits<struct item>
{
public:
    static const int TIME_INDEX = 0;
    static const int AMOUNT_INDEX = 1;
    static const int ACCOUNT_INDEX = 2;
    static const int DESC_INDEX = 3;
    static const int VALID_INDEX = 4;
    static const int BATCH_INDEX = 5;

    static const int cols = 6;

    static const ColumnType constexpr types[] = {
        TIME,
        MONEY,
        INT32,
        CSTR,
        BOOL,
        INT32,
    };

    static void *getPtr(void *data, int i)
    {
        struct item *item = static_cast<struct item *>(data);
        switch (i) {
        case TIME_INDEX:
            return &item->time;
        case AMOUNT_INDEX:
            return &item->amount;
        case ACCOUNT_INDEX:
            return &item->account;
        case DESC_INDEX:
            return &item->desc;
        case VALID_INDEX:
            return &item->valid;
        case BATCH_INDEX:
            return &item->batch;
        default:
            break;
        }
        return nullptr;
    }
};

typedef CsvRowTraits<struct item> ItemTraits;

#endif /* _DATA_ITEM_TRAITS_H_ */
