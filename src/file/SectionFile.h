#ifndef _FILE_SECTION_FILE_H_
#define _FILE_SECTION_FILE_H_

#include <map>
#include <string>

class SectionStore;

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

class SectionFile
{
public:
    SectionFile() : m_cache(), m_store(nullptr)
    {
    }

    SectionFile(SectionStore *store) : m_cache(), m_store(store)
    {
    }

    virtual ~SectionFile();

    const std::string &get(const std::string &name);
    void put(const std::string &name, const std::string &content);
    void remove(const std::string &name);

    void attach(SectionStore *store);
    void save();
    void saveAs(SectionStore *store);

    void getSectionNames(std::vector<const std::string> &names) const;

private:
    std::map<std::string, Section> m_cache;
    SectionStore *m_store;
};

#endif /* _FILE_SECTION_FILE_H_ */
