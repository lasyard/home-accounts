#ifndef _CSV_WRAPPER_H_
#define _CSV_WRAPPER_H_

#include <stddef.h>

#include "ColumnType.h"

class CsvParser;

struct string;

class Wrapper
{
public:
    typedef bool ConfigParser(const struct string *str, ColumnType *type, int *index);

    Wrapper(const char *config, ConfigParser *parser);
    virtual ~Wrapper();

    CsvParser *createParser() const;

    void setData(void *data)
    {
        m_data = data;
    }

private:
    static const char SEPS_SEP = ':';
    static const char COLS_SEP = '/';

    char m_seps[3];
    int m_cols;
    ColumnType *m_types;
    int *m_map;
    void *m_data;

    static void *getPtr(void *data, int i);

protected:
    virtual void *getDataPtr(void *data, int i) = 0;
};

#endif /* _CSV_WRAPPER_H_ */
