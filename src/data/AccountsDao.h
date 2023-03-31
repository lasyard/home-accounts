#ifndef _DATA_ACCOUNTS_DAO_H_
#define _DATA_ACCOUNTS_DAO_H_

#include "ConfigPodsTraits.h"
#include "CsvIdVecDao.h"
#include "Joint.h"

class AccountsDao : public CsvIdVecDao<struct account, 0>
{
public:
    AccountsDao();
    virtual ~AccountsDao();

    void setOwerJoint(Joint<const char *, int32_t> owerJoint)
    {
        m_ownerJoint = owerJoint;
    }

    std::string getString(int row, int col) override;
    void setString(int row, int col, const std::string &value) override;

private:
    Joint<const char *, int32_t> m_ownerJoint;
};

#endif /* _DATA_ACCOUNTS_DAO_H_ */
