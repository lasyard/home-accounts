#include "AccountsDao.h"

AccountsDao::AccountsDao() : CsvIdVecDao<struct account, 0>(), m_ownerJoint()
{
}

AccountsDao::~AccountsDao()
{
}

std::string AccountsDao::getString(int row, int col)
{
    const struct account &item = m_data[row];
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        return wrapString(m_ownerJoint.lookup(item.owner));
    } else if (col == CsvRowTraits<struct account>::TYPE_INDEX) {
        return wrapString(m_typeJoint.lookup(item.type));
    }
    return CsvIdVecDao<struct account, 0>::getString(row, col);
}

void AccountsDao::setString(int row, int col, const std::string &value)
{
    struct account &item = m_data[row];
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        item.owner = m_ownerJoint.revLookup(value.c_str());
    } else if (col == CsvRowTraits<struct account>::TYPE_INDEX) {
        item.type = m_typeJoint.revLookup(value.c_str());
    } else {
        CsvIdVecDao<struct account, 0>::setString(row, col, value);
    }
}
