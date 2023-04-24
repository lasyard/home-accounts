#ifndef _DATA_DATA_IMPORT_DAO_H_
#define _DATA_DATA_IMPORT_DAO_H_

#include "DataDao.h"
#include "ItemTraits.h"

class DataImportDao : public DataDao
{
public:
    DataImportDao();
    virtual ~DataImportDao();

    void read(std::istream &is) override;

private:
    class ItemWrap
    {
    public:
        static const int DATE_INDEX = ItemTraits::cols;

        int cols;
        int map[ItemTraits::cols];
        ColumnType types[ItemTraits::cols];
        date_t date;
        struct item *item;

        static void *getPtr(void *data, int i);
    } m_wrap;

    void parseHeader();
};

#endif /* _DATA_DATA_IMPORT_DAO_H_ */
