#ifndef _DATA_DATA_IMPORT_DAO_H_
#define _DATA_DATA_IMPORT_DAO_H_

#include "DataDao.h"
#include "ItemWrap.h"

class DataImportDao : public DataDao
{
public:
    DataImportDao();
    virtual ~DataImportDao();

    void read(std::istream &is) override;

private:
    ItemWrap m_wrap;
};

#endif /* _DATA_DATA_IMPORT_DAO_H_ */
