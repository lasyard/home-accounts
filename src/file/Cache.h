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

    template <typename F> void forEach(F &&callback) const
    {
        for (auto it = m_cache.begin(); it != m_cache.end();) {
            auto name = it->first;
            // advance first, so the loop iterator is still valid if it is erased
            ++it;
            if (!std::forward<F>(callback)(name)) {
                break;
            }
        }
    }

    Store *getStore() const
    {
        return m_store;
    }

private:
    std::map<std::string, Section> m_cache;
    Store *m_store;
};

#endif /* _HA_FILE_CACHE_H_ */
