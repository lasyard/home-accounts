#ifndef _FILE_SECTION_STORE_H_
#define _FILE_SECTION_STORE_H_

#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Crypto.h"

class SectionStore
{
public:
    SectionStore()
    {
    }

    virtual ~SectionStore()
    {
    }

    virtual void create() = 0;
    virtual void open() = 0;
    virtual void clear() = 0;

    virtual void flush()
    {
    }

    virtual void readSection(const std::string &name, std::ostream &content) = 0;
    virtual void readSection(const std::string &name, std::string &content) = 0;
    virtual void writeSection(const std::string &name, std::istream &content) = 0;
    virtual void writeSection(const std::string &name, const std::string &content) = 0;
    virtual void deleteSection(const std::string &name) = 0;

    virtual void getSectionNames(std::vector<const std::string> &names) const = 0;

    virtual bool contains(const std::string &name) const = 0;
    virtual bool operator==(const SectionStore &obj) const = 0;

    // GCC 9.4 cannot induce this method.
    virtual bool operator!=(const SectionStore &obj) const
    {
        return !operator==(obj);
    }
};

#endif /* _FILE_SECTION_STORE_H_ */
