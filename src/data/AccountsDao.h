#ifndef _DATA_ACCOUNTS_DAO_H_
#define _DATA_ACCOUNTS_DAO_H_

#include "ConfigPodsTraits.h"
#include "CsvIdVecDao.h"

template <typename T, typename S> class Joint;

class AccountsDao : public CsvIdVecDao<struct account, 0>
{
public:
    AccountsDao(const std::string &name = "");
    virtual ~AccountsDao();

    std::string getString(int row, int col) override;
    void setString(int row, int col, const std::string &value) override;

    void setOwnerJoint(Joint<const char *, int32_t> *ownerJoint);
    void setTypeJoint(Joint<const char *, int32_t> *typeJoint);

    const Joint<const char *, int32_t> *getOwnerJoint() const
    {
        return m_ownerJoint;
    }

    const Joint<const char *, int32_t> *getTypeJoint() const
    {
        return m_typeJoint;
    }

private:
    Joint<const char *, int32_t> *m_ownerJoint;
    Joint<const char *, int32_t> *m_typeJoint;
};

#endif /* _DATA_ACCOUNTS_DAO_H_ */
