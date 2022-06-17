#include "SectionFile.h"
#include "FileExeptions.h"
#include "SectionStore.h"

SectionFile::~SectionFile()
{
    if (m_store != nullptr) {
        delete m_store;
    }
}

const std::string &SectionFile::get(const std::string &name)
{
    auto &section = m_cache.at(name);
    if (section.readDirty && m_store != nullptr) {
        m_store->readSection(name, section.content);
    }
    section.readDirty = false;
    return section.content;
}

void SectionFile::put(const std::string &name, const std::string &content)
{
    auto &section = m_cache[name];
    section.content = content;
    section.writeDirty = true;
}

void SectionFile::remove(const std::string &name)
{
    if (m_store != nullptr) {
        m_store->deleteSection(name);
    }
    m_cache.erase(name);
}

SectionFile *SectionFile::attach(SectionStore *store)
{
    if (m_store != nullptr) {
        delete m_store;
    }
    store->open();
    m_store = store;
    m_store->forEachSection([this](const std::string &name) {
        m_cache[name] = Section();
        return true;
    });
    return this;
}

SectionFile *SectionFile::save()
{
    if (m_store == nullptr) {
        throw NoFileSpecified();
    }
    for (auto &[name, section] : m_cache) {
        if (section.writeDirty) {
            m_store->writeSection(name, section.content);
            section.writeDirty = false;
        }
    }
    m_store->flush();
    return this;
}

SectionFile *SectionFile::saveAs(SectionStore *store)
{
    if (m_store != nullptr) {
        if (*m_store != *store) {
            for (auto &[name, section] : m_cache) {
                if (section.readDirty) {
                    m_store->readSection(name, section.content);
                }
            }
            delete m_store;
            store->create();
            m_store = store;
        }
    } else {
        store->create();
        m_store = store;
    }
    save();
    return this;
}
