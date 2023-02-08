#ifndef _DATA_CSV_DAO_H_
#define _DATA_CSV_DAO_H_

#include <vector>

#include "Dao.h"
#include "csv/ColumnType.h"
#include "csv/CsvParser.h"

template <typename T> class CsvDao : public Dao<T>
{
public:
    CsvDao(
        int cols,
        const ColumnType types[],
        void *(*readPtr)(void *data, int i),
        const void *(*writePtr)(const void *data, int i)
    )
        : Dao<T>()
    {
        m_parser = new CsvParser(cols, types, readPtr, writePtr);
    }

    virtual ~CsvDao()
    {
        delete m_parser;
    }

    int getNumberCols() const
    {
        return m_parser->getCols();
    }

protected:
    CsvParser *m_parser;
};

#endif /* _DATA_CSV_DAO_H_ */
