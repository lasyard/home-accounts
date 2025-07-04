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

    const wxString GetSegmentValueString(const struct segment *segment) const override;
    void SetSegmentValueString(struct segment *segment, const wxString &value) override;

    struct account *GetAccount(int id) const;

private:
    static const wxArrayString ACCOUNT_TYPE_STRINGS;

    mutable int m_maxId;

    bool AfterRead() override;

    void SetNewItem(void *item) override;
};

#endif /* _HA_ACCOUNT_ACCOUNT_DOC_H_ */
