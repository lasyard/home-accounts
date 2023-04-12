#ifndef _DATA_DAO_H_
#define _DATA_DAO_H_

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

class DaoBase
{
protected:
    DaoBase()
    {
    }

    virtual ~DaoBase()
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
};

template <typename T> class Dao : public DaoBase
{
public:
    Dao() : DaoBase()
    {
    }

    virtual ~Dao()
    {
    }

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
