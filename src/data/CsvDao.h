#ifndef _DATA_CSV_DAO_H_
#define _DATA_CSV_DAO_H_

#include <vector>

#include "CsvRowTraits.h"
#include "Dao.h"

#include "csv/CsvParser.h"

template <typename I, typename T> class CsvDao : public Dao<T>
{
protected:
    using Traits = CsvRowTraits<I>;

public:
    CsvDao(const std::string &name = "") : Dao<T>(name)
    {
        m_parser = new CsvParser(Traits::cols, Traits::types, Traits::getPtr);
    }

    virtual ~CsvDao()
    {
        delete m_parser;
    }

    int getNumberCols() const
    {
        return m_parser->getCols();
    }

    void read(std::istream &is) override = 0;
    void write(std::ostream &os) const override = 0;

protected:
    static const int MAX_LINE_LENGTH = 1024;

    CsvParser *m_parser;
    mutable char m_buf[MAX_LINE_LENGTH];
};

#endif /* _DATA_CSV_DAO_H_ */
