#include "AccountsDao.h"

AccountsDao::AccountsDao() : CsvIdVecDao<struct account, 0>(), m_ownerJoint()
{
}

AccountsDao::~AccountsDao()
{
}

std::string AccountsDao::getString(int row, int col)
{
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        const struct account &item = m_data[row];
        return wrapString(m_ownerJoint.lookup(item.owner));
    }
    return CsvIdVecDao<struct account, 0>::getString(row, col);
}

void AccountsDao::setString(int row, int col, const std::string &value)
{
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        struct account &item = m_data[row];
        item.owner = m_ownerJoint.revLookup(value.c_str());
    } else {
        CsvIdVecDao<struct account, 0>::setString(row, col, value);
    }
}
