#include "Cache.h"
#include "Exceptions.h"
#include "Store.h"

Cache::Cache() : m_cache(), m_store(nullptr)
{
}

Cache::Cache(Store *store) : m_cache(), m_store(store)
{
    store->open();
    m_store->forEachSection([this](const std::string &name) -> bool {
        m_cache[name] = Section();
        return true;
    });
}

Cache::~Cache()
{
    save();
    if (m_store != nullptr) {
        delete m_store;
    }
}

const std::string &Cache::get(const std::string &name)
{
    try {
        auto &section = m_cache.at(name);
        if (m_store != nullptr && section.readDirty) {
            m_store->readSection(name, section.content);
        }
        section.readDirty = false;
        return section.content;
    } catch (std::out_of_range &) {
        throw SectionNotFound(name);
    }
}

void Cache::put(const std::string &name, const std::string &content)
{
    auto &section = m_cache[name];
    section.content = content;
    section.writeDirty = true;
    section.readDirty = false;
}

void Cache::remove(const std::string &name)
{
    if (m_store != nullptr) {
        m_store->deleteSection(name);
    }
    m_cache.erase(name);
}

void Cache::removePrefix(const std::string &prefix)
{
    if (m_store != nullptr) {
        m_store->deleteSectionPrefix(prefix);
    }
    for (auto &[name, section] : m_cache) {
        if (name.starts_with(prefix)) {
            m_cache.erase(name);
        }
    }
}

void Cache::removeAll()
{
    if (m_store != nullptr) {
        m_store->deleteAllSections();
    }
    m_cache.clear();
}

void Cache::save()
{
    if (m_store == nullptr) {
        return;
    }
    for (auto &[name, section] : m_cache) {
        if (section.writeDirty) {
            m_store->writeSection(name, section.content);
            section.writeDirty = false;
        }
    }
    m_store->flush();
}

void Cache::saveAs(Store *store)
{
    if (store == nullptr) {
        return;
    }
    if (m_store == nullptr || *m_store != *store) {
        for (auto &[name, section] : m_cache) {
            if (section.readDirty) {
                m_store->readSection(name, section.content);
            }
        }
        try {
            store->create();
            delete m_store;
            m_store = store;
        } catch (FileOpen &e) {
            throw e;
        }
        save();
    }
}

void Cache::forEach(std::function<bool(const std::string &)> callback) const
{
    for (auto it = m_cache.begin(); it != m_cache.end();) {
        auto name = it->first;
        // advance first, so the loop iterator is still valid if it is erased
        ++it;
        if (!callback(name)) {
            break;
        }
    }
}
