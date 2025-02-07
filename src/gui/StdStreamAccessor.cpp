#include "StdStreamAccessor.h"

#include <istream>

#include "csv/csv_parser.h"

extern "C" const char *stream_reader(void *context)
{
    static char buf[MAX_LINE_LENGTH + 1];
    std::istream *is = static_cast<std::istream *>(context);
    if (!is->eof()) {
        is->getline(buf, MAX_LINE_LENGTH);
        return buf;
    }
    return NULL;
}

extern "C" void stream_writer(void *context, const char *buf, size_t len)
{
    std::ostream *os = static_cast<std::ostream *>(context);
    os->write(buf, len);
    os->put('\n');
}
