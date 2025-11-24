#ifndef _HA_FILE_CACHE_H_
#define _HA_FILE_CACHE_H_

#include <functional>
#include <map>
#include <string>

#include "Store.h"

class Section
{
public:
    Section() : content(), readDirty(true), writeDirty(false)
    {
    }

public:
    std::string content;
    bool readDirty;
    bool writeDirty;
};

class Cache
{
public:
    Cache();
    Cache(Store *store);
    virtual ~Cache();

    const std::string &get(const std::string &name);
    void put(const std::string &name, const std::string &content);
    void remove(const std::string &name);
    void removePrefix(const std::string &prefix);
    void removeAll();

    void save();
    void saveAs(Store *store);

    void forEach(std::function<bool(const std::string &)> callback) const;

    Store *getStore() const
    {
        return m_store;
    }

private:
    std::map<std::string, Section> m_cache;
    Store *m_store;
};

#endif /* _HA_FILE_CACHE_H_ */
