#include "account.h"

#include "csv/csv_parser.h"
#include "csv/segment.h"

const enum column_type account_types[ACCOUNT_COLS] = {
    CT_INT32, // ID
    CT_CSTR,  // Name
    CT_DATE,  // Open Date
    CT_MONEY, // Initial
    CT_CSTR,  // Memo
};

void *account_get(void *data, int i, const void *context)
{
    (void)context;
    struct account *a = (struct account *)data;
    switch (i) {
    case LIST_ITEM_INDEX:
        return &a->list;
    case ACCOUNT_ID_COL: // ID
        return &a->id;
    case ACCOUNT_NAME_COL: // Name
        return &a->name;
    case ACCOUNT_BEGIN_COL: // Open Date
        return &a->begin;
    case ACCOUNT_INITIAL_COL: // Initial
        return &a->initial;
    case ACCOUNT_MEMO_COL: // Memo
        return &a->memo;
    default:
        break;
    }
    return NULL;
}

const struct segment *fill_account_types(struct list_head *segments, int32_t *maxId)
{
    const struct segment *segment =
        fill_serial(segments, ACCOUNT_TYPE_DEBIT, ACCOUNT_TYPE_TRANSFER, get_segment_int, set_segment_int);
    if (segment == NULL) {
        for (struct list_item *p = segments->first; p != NULL; p = p->next) {
            struct segment *s = get_segment(p);
            for (struct list_item *q = s->items.first; q != NULL; q = q->next) {
                struct account *a = get_account(q);
                a->type = get_segment_int(s);
                if (a->id > *maxId) {
                    *maxId = a->id;
                }
            }
        }
    }
    return segment;
}
