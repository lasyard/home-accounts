#ifndef _HA_ACCOUNT_ACCOUNT_DOC_H_
#define _HA_ACCOUNT_ACCOUNT_DOC_H_

#include "../CsvDoc.h"

class AccountDoc : public CsvDoc
{
public:
    DECLARE_TM()

    AccountDoc();
    virtual ~AccountDoc();

private:
    bool AfterRead() override;
};

#endif /* _HA_ACCOUNT_ACCOUNT_DOC_H_ */
