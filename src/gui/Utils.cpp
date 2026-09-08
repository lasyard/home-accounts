#include <istream>
#include <ostream>

#include "Utils.h"

int get_line_from_istream(char *buf, size_t len, void *context)
{
    std::istream *is = static_cast<std::istream *>(context);
    if (is->eof()) {
        return -1;
    }
    is->getline(buf, len);
    return (int)is->gcount();
}

int put_line_to_ostream(const char *buf, size_t len, void *context)
{
    std::ostream *os = static_cast<std::ostream *>(context);
    os->write(buf, len);
    os->put('\n');
    return (int)len;
}

bool record_is_hash(const struct parser *parser, const record_t *record)
{
    (void)parser;
    return record->flag == RECORD_FLAG_HASH;
}

int get_record_first_int64(const struct parser *parser, const record_t *record)
{
    return (int)*(int64_t *)get_const_field(parser, record, 0);
}

int get_record_first_int32(const struct parser *parser, const record_t *record)
{
    return (int)*(int32_t *)get_const_field(parser, record, 0);
}

record_t *new_hash_record_int64(const struct parser *parser, int expected)
{
    record_t *record = new_record(parser);
    return_null_if_null(record);
    *(int64_t *)get_field(parser, record, 0) = (int64_t)expected;
    record->flag = RECORD_FLAG_HASH;
    return record;
}

record_t *new_hash_record_int32(const struct parser *parser, int expected)
{
    record_t *record = new_record(parser);
    return_null_if_null(record);
    *(int32_t *)get_field(parser, record, 0) = (int32_t)expected;
    record->flag = RECORD_FLAG_HASH;
    return record;
}
