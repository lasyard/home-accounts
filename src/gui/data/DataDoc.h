#ifndef _HA_DATA_DATA_DOC_H_
#define _HA_DATA_DATA_DOC_H_

#include "../CsvDoc.h"

class DataDoc : public CsvDoc
{
public:
    DECLARE_TM()

    DataDoc(int year, int month);
    virtual ~DataDoc();

private:
    static const char *const m_labels[];
    static const enum column_type m_types[];

    int m_year;
    int m_month;

    bool AfterRead() override;
    bool BeforeWrite() override;
};

#endif /* _HA_DATA_DATA_DOC_H_ */
