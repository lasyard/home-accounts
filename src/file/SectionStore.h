#ifndef _FILE_SECTION_STORE_H_
#define _FILE_SECTION_STORE_H_

#include <functional>
#include <map>
#include <sstream>
#include <string>

class SectionStore
{
public:
    SectionStore()
    {
    }

    virtual ~SectionStore()
    {
    }

    virtual SectionStore *create() = 0;
    virtual SectionStore *open() = 0;
    virtual SectionStore *readSection(const std::string &name, std::ostream &content) = 0;
    virtual SectionStore *readSection(const std::string &name, std::string &content) = 0;
    virtual SectionStore *writeSection(const std::string &name, std::istream &content) = 0;
    virtual SectionStore *writeSection(const std::string &name, const std::string &content) = 0;
    virtual SectionStore *deleteSection(const std::string &name) = 0;
    virtual SectionStore *clear() = 0;
    virtual SectionStore *flush() = 0;

    virtual bool contains(const std::string &name) const = 0;
    virtual bool operator==(const SectionStore &obj) const = 0;

    // GCC 9.4 cannot induce this method.
    virtual bool operator!=(const SectionStore &obj) const
    {
        return !operator==(obj);
    }

    virtual void forEachSection(std::function<bool(const std::string &name)> fun) = 0;

private:
};

#endif /* _FILE_SECTION_STORE_H_ */
