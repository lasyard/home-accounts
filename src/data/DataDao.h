#ifndef _DATA_DATA_DAO_H_
#define _DATA_DATA_DAO_H_

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "CsvDao.h"

#include "data.h"

template <typename T, typename S> class Joint;

class DataDao : public CsvDao<struct item, struct data>
{
public:
    DataDao();
    virtual ~DataDao();

    enum IndexType {
        INITIAL,
        PAGE,
        ITEM,
        OTHER,
    };

    void setName(const std::string &name)
    {
        m_name = name;
    }

    void read(std::istream &is) override;
    void write(std::ostream &os) const override;

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
    std::string getBalanceString(int row);
    std::string getValidString(int row);

    std::string getTotalIncomeString();
    std::string getTotalOutlayString();

    void setMoney(int row, const std::string &value, bool negative);
    void setAccount(int row, const std::string &value);
    void setChannel(int row, const std::string &value);
    void setDesc(int row, const std::string &value);
    void setValid(int row, const std::string &value);

    bool insertItemAfter(size_t pos);

    void setInitialBalance(money_t balance);

    bool isRedBalance(int row) const;

    void setAccountJoint(Joint<const char *, int32_t> *joint);
    void setChannelJoint(Joint<const char *, int32_t> *joint);

    const Joint<const char *, int32_t> *getAccountJoint() const
    {
        return m_accountJoint;
    }

    const Joint<const char *, int32_t> *getChannelJoint() const
    {
        return m_channelJoint;
    }

    bool isEmpty() const override
    {
        return data_is_empty(&m_data);
    }

    void fillMissingPages(date_t min, date_t max)
    {
        if (fill_missing_pages(&m_data, min, max)) {
            createIndex();
        }
    }

    IndexType getRowType(int row) const
    {
        return m_index[row].m_type;
    }

protected:
    void createIndex();
    struct page *findPage(date_t date);

private:
    struct IndexItem {
        explicit IndexItem(struct page *ptr) : m_ptr(ptr), m_type(PAGE), m_seq(0), m_balance(0)
        {
        }

        explicit IndexItem(struct item *ptr, int seq, money_t balance)
            : m_ptr(ptr)
            , m_type(ITEM)
            , m_seq(seq)
            , m_balance(balance)
        {
        }

        explicit IndexItem(money_t balance) : m_ptr(nullptr), m_type(INITIAL), m_seq(0), m_balance(balance)
        {
        }

        void *m_ptr;
        enum IndexType m_type;
        int m_seq;
        money_t m_balance;
    };

    std::vector<struct IndexItem> m_index;
    Joint<const char *, int32_t> *m_accountJoint;
    Joint<const char *, int32_t> *m_channelJoint;
    money_t m_totalIncome;
    money_t m_totalOutlay;

    struct item *safeGetItem(int row)
    {
        if (m_index[row].m_type == ITEM) {
            return static_cast<struct item *>(m_index[row].m_ptr);
        }
        return nullptr;
    }

    void updateBalance(int row, money_t balance);
    void updateTotal();
    void readPage(struct page *page);
    void readItem(struct item *item);
    void writePage(std::ostream &os, const struct page *page) const;
    void writeItem(std::ostream &os, const struct item *item) const;
};

#endif /* _DATA_DATA_DAO_H_ */
