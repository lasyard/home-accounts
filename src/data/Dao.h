#ifndef _DATA_DAO_H_
#define _DATA_DAO_H_

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

template <typename T> class Dao
{
public:
    Dao()
    {
    }

    virtual ~Dao()
    {
    }

    virtual void read(std::istream &is) = 0;
    virtual void write(std::ostream &os) = 0;

    T &getData()
    {
        return m_data;
    }

    const T &getData() const
    {
        return m_data;
    }

    virtual void readString(const std::string &str)
    {
        std::istringstream is(str);
        read(is);
    }

    virtual void writeString(std::string &str)
    {
        std::ostringstream os;
        write(os);
        str = os.str();
    }

protected:
    T m_data;
};

#endif /* _DATA_DAO_H_ */
