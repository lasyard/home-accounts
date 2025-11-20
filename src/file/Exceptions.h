#ifndef _HA_FILE_EXCEPTIONS_H_
#define _HA_FILE_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

class FileOpen : public std::runtime_error
{
public:
    explicit FileOpen(const std::string &fileName) : std::runtime_error("cannot open/create file \"" + fileName + "\"")
    {
    }
};

class FileCorrupt : public std::runtime_error
{
public:
    explicit FileCorrupt(const std::string &what) : std::runtime_error(what)
    {
    }
};

class BadPassword : public std::runtime_error
{
public:
    explicit BadPassword() : std::runtime_error("bad password")
    {
    }
};

class SectionNotFound : public std::runtime_error
{
public:
    explicit SectionNotFound(const std::string &name)
        : std::runtime_error("section \"" + name + "\" is not found")
        , m_name(name)
    {
    }

    const std::string &getName() const
    {
        return m_name;
    }

private:
    std::string m_name;
};

#endif /* _HA_FILE_EXCEPTIONS_H_ */
