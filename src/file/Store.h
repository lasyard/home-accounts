#ifndef _HA_FILE_STORE_H_
#define _HA_FILE_STORE_H_

#include <functional>
#include <string>

class Store
{
public:
    Store()
    {
    }

    virtual ~Store()
    {
    }

    virtual void create() = 0;
    virtual void open() = 0;
    virtual void clear() = 0;

    virtual void flush()
    {
    }

    virtual void readSection(const std::string &name, std::string &content) = 0;
    virtual void writeSection(const std::string &name, const std::string &content) = 0;
    virtual void deleteSection(const std::string &name) = 0;

    virtual void forEachSection(std::function<bool(const std::string &)> callback) const = 0;

    virtual void deleteSectionPrefix(const std::string &prefix)
    {
        forEachSection([this, prefix](const std::string &name) -> bool {
            if (name.starts_with(prefix)) {
                deleteSection(name);
            }
            return true;
        });
    }

    virtual void deleteAllSections()
    {
        forEachSection([this](const std::string &name) -> bool {
            deleteSection(name);
            return true;
        });
    }

    virtual bool contains(const std::string &name) const = 0;
    virtual bool operator==(const Store &obj) const = 0;

    // GCC 9.4 cannot induce this method
    virtual bool operator!=(const Store &obj) const
    {
        return !operator==(obj);
    }
};

#endif /* _HA_FILE_STORE_H_ */
