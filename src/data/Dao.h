#ifndef _DATA_DAO_H_
#define _DATA_DAO_H_

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

class DaoBase
{
protected:
    DaoBase(const std::string &name = "") : m_name(name)
    {
    }

    virtual ~DaoBase()
    {
    }

private:
    DaoBase([[maybe_unused]] const DaoBase &dao)
    {
    }

public:
    static std::string wrapString(const char *str)
    {
        return (str != nullptr) ? std::string(str) : std::string();
    }

    virtual void read(std::istream &is) = 0;
    virtual void write(std::ostream &os) const = 0;

    virtual int getNumberRows() const = 0;

    virtual bool isEmpty() const
    {
        return false;
    }

    virtual void readString(const std::string &str)
    {
        std::istringstream is(str);
        read(is);
    }

    virtual void writeString(std::string &str) const
    {
        std::ostringstream os;
        write(os);
        str = os.str();
    }

    const std::string &getName() const
    {
        return m_name;
    }

protected:
    std::string m_name;
};

template <typename T> class Dao : public DaoBase
{
protected:
    Dao(const std::string &name = "") : DaoBase(name)
    {
    }

    virtual ~Dao()
    {
    }

public:
    T &getData()
    {
        return m_data;
    }

    const T &getData() const
    {
        return m_data;
    }

protected:
    T m_data;
};

#endif /* _DATA_DAO_H_ */
