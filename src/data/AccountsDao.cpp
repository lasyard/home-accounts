#include "AccountsDao.h"

#include "CxxDefs.h"
#include "Joint.h"

AccountsDao::AccountsDao(const std::string &name)
    : CsvIdVecDao<struct account, 0>(name)
    , m_ownerJoint(nullptr)
    , m_typeJoint(nullptr)
{
}

AccountsDao::~AccountsDao()
{
    safe_delete(m_ownerJoint);
    safe_delete(m_typeJoint);
}

std::string AccountsDao::getString(int row, int col)
{
    const struct account &item = m_data[row];
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        return wrapString(m_ownerJoint->lookup(item.owner));
    } else if (col == CsvRowTraits<struct account>::TYPE_INDEX) {
        return wrapString(m_typeJoint->lookup(item.type));
    }
    return CsvIdVecDao<struct account, 0>::getString(row, col);
}

void AccountsDao::setString(int row, int col, const std::string &value)
{
    struct account &item = m_data[row];
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        item.owner = m_ownerJoint->revLookup(value.c_str());
    } else if (col == CsvRowTraits<struct account>::TYPE_INDEX) {
        item.type = m_typeJoint->revLookup(value.c_str());
    } else {
        CsvIdVecDao<struct account, 0>::setString(row, col, value);
    }
}

void AccountsDao::setOwnerJoint(Joint<const char *, int32_t> *ownerJoint)
{
    safe_delete(m_ownerJoint);
    m_ownerJoint = ownerJoint;
}

void AccountsDao::setTypeJoint(Joint<const char *, int32_t> *typeJoint)
{
    safe_delete(m_typeJoint);
    m_typeJoint = typeJoint;
}
