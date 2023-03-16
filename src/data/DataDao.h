#ifndef _DATA_DATA_DAO_H_
#define _DATA_DATA_DAO_H_

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "CsvDao.h"
#include "data.h"

class DataDao : public CsvDao<struct item, struct data>
{
public:
    DataDao();
    virtual ~DataDao();

    enum IndexType {
        PAGE,
        ITEM,
    };

    typedef std::function<const char *(int)> LookupCallback;
    typedef std::function<int(const char *)> RevLookupCallback;

    void read(std::istream &is) override;
    void write(std::ostream &os) override;

    int getNumberRows() const override
    {
        return m_index.size();
    }

    std::string getRowLabel(int row);
    std::string getPageTitleString(int row);
    std::string getTimeString(int row);
    std::string getIncomeString(int row);
    std::string getOutlayString(int row);
    std::string getAccountString(int row);
    std::string getChannelString(int row);
    std::string getDescString(int row);
    std::string getValidString(int row);

    void setMoney(int row, const std::string &value, bool negative);
    void setAccount(int row, const std::string &value);
    void setChannel(int row, const std::string &value);
    void setDesc(int row, const std::string &value);
    void setValid(int row, const std::string &value);

    bool insertItemAfter(size_t pos);

    IndexType getRowType(int row) const
    {
        return m_index[row].m_type;
    }

    void setAccountLookup(LookupCallback f1, RevLookupCallback f2)
    {
        m_accountLookup = f1;
        m_accountRevLookup = f2;
    }

    void setChannelLookup(LookupCallback f1, RevLookupCallback f2)
    {
        m_channelLookup = f1;
        m_channelRevLookup = f2;
    }

private:
    struct IndexItem {
        IndexItem(void *ptr, enum IndexType type, int seq) : m_ptr(ptr), m_type(type), m_seq(seq)
        {
        }

        void *m_ptr;
        enum IndexType m_type;
        int m_seq;
    };

    std::vector<struct IndexItem> m_index;
    LookupCallback m_accountLookup;
    LookupCallback m_channelLookup;
    RevLookupCallback m_accountRevLookup;
    RevLookupCallback m_channelRevLookup;

    static std::string lookupId(LookupCallback f, int id)
    {
        if (f != nullptr) {
            const char *s = f(id);
            if (s != nullptr) {
                return s;
            }
        }
        return "N/A (" + std::to_string(id) + ")";
    }

    static int lookupName(RevLookupCallback f, const std::string &name)
    {
        if (f != nullptr) {
            return f(name.c_str());
        }
        return 0;
    }

    struct item *safeGetItem(int row)
    {
        if (m_index[row].m_type == ITEM) {
            return static_cast<struct item *>(m_index[row].m_ptr);
        }
        return nullptr;
    }

    void createIndex();
    void readPage(struct page *page);
    void readItem(struct item *item);
    void writePage(std::ostream &os, const struct page *page);
    void writeItem(std::ostream &os, const struct item *item);
};

#endif /* _DATA_DATA_DAO_H_ */
