#ifndef _HA_ACCOUNT_ACCOUNT_DOC_H_
#define _HA_ACCOUNT_ACCOUNT_DOC_H_

#include "../CsvDoc.h"

#include "data/account.h"

class AccountDoc : public CsvDoc
{
public:
    DECLARE_TM()

    AccountDoc();
    virtual ~AccountDoc();

    struct account *GetAccount(int id) const;

private:
    mutable int m_maxId;

    bool AfterRead() override;

    void SetNewItem(void *item) override;
};

#endif /* _HA_ACCOUNT_ACCOUNT_DOC_H_ */
