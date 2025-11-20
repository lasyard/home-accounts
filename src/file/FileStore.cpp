#include <sstream>

#include "Exceptions.h"
#include "FileStore.h"

const char *const FileStore::CATALOG_NAME = "__catalog__";

FileStore::FileStore(const std::string &fileName, const std::string &pass, const std::string &iv)
    : CryptoStore(iv)
    , m_fileName(fileName)
    , m_file()
    , m_catalog()
{
    setKey(m_key, pass);
}

FileStore::~FileStore()
{
    if (m_file.is_open()) {
        flush();
        m_file.close();
    }
}

void FileStore::create()
{
    m_file.open(m_fileName, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    if (!m_file.is_open()) {
        throw FileOpen(m_fileName);
    }
    clear();
}

void FileStore::open()
{
    if (m_file.is_open()) {
        return;
    }
    m_file.open(m_fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!m_file.is_open()) {
        throw FileOpen(m_fileName);
    }
    loadCatalog();
}

void FileStore::clear()
{
    // clear all sections
    m_catalog.clear();
    // must not be empty, or it is treated as a new section
    m_catalog[CATALOG_NAME] = SectionRecord(CATALOG_NAME);
    // ocuppy the slot
    saveCatalog();
}

void FileStore::flush()
{
    saveCatalog();
    m_file.flush();
}

void FileStore::deleteSection(const std::string &name)
{
    m_catalog.erase(name);
}

void FileStore::forEachSection(std::function<bool(const std::string &)> callback) const
{
    for (auto it = m_catalog.begin(); it != m_catalog.end();) {
        auto name = it->first;
        // advance first, so the loop iterator is still valid if it is erased
        ++it;
        if (name == CATALOG_NAME) {
            continue;
        }
        if (!callback(name)) {
            break;
        }
    }
}

bool FileStore::contains(const std::string &name) const
{
    return m_catalog.find(name) != m_catalog.end();
}

bool FileStore::operator==(const Store &obj) const
{
    try {
        const FileStore &file = dynamic_cast<const FileStore &>(obj);
        return file.m_fileName == m_fileName;
    } catch ([[maybe_unused]] std::bad_cast &e) {
        return false;
    }
}

void FileStore::changePass(const std::string &pass)
{
    setKey(m_key, pass);
}

const byte *FileStore::readRawSection(const std::string &name, size_t &size, byte key[CRYPTO_KEY_LEN])
{
    SectionRecord *sr = nullptr;
    try {
        sr = &m_catalog.at(name);
    } catch (std::out_of_range &) {
        return nullptr;
    }
    size = sr->size;
    byte *buf = new byte[size];
    m_file.seekg(sr->offset);
    m_file.read((char *)buf, size);
    if ((size_t)m_file.gcount() < size) {
        throw FileCorrupt("section is too short");
    }
    copyKey(key, sr->key);
    return buf;
}

void FileStore::writeRawSection(const std::string &name, const std::string &content, const byte key[CRYPTO_KEY_LEN])
{
    SectionRecord sr(name);
    copyKey(sr.key, key);
    sr.size = content.length();
    sr.offset = findSlot(sr.size);
    m_file.seekp(sr.offset);
    m_file.write(content.c_str(), sr.size);
    m_catalog[name] = sr;
}

void FileStore::loadCatalog()
{
    m_catalog.clear();
    m_file.seekg(0);
    // read catalog info
    SectionRecord sr;
    if (!sr.read(m_file)) {
        throw FileCorrupt("CATALOG is too short");
    }
    xorKey(sr.key, m_key);
    m_catalog[CATALOG_NAME] = sr;
    // read catalog
    std::string tmp;
    readSection(CATALOG_NAME, tmp);
    // read info of sections
    std::istringstream content(tmp);
    while (sr.read(content)) {
        m_catalog[sr.name] = sr;
    }
}

void FileStore::saveCatalog()
{
    std::ostringstream content;
    for (auto &[name, sr] : m_catalog) {
        // do not save catalog here, or reading it will mess up the real catalog[CATALOG_NAME]
        if (name == CATALOG_NAME) {
            continue;
        }
        sr.write(content);
    }
    writeSection(CATALOG_NAME, content.str());
    SectionRecord sr = m_catalog[CATALOG_NAME];
    // don't write pass into file
    xorKey(sr.key, m_key);
    m_file.seekp(0);
    sr.write(m_file);
}

size_t FileStore::findSlot(size_t size) const
{
    std::vector<const SectionRecord *> vec;
    // must reference to get the real address in `m_catalog`
    for (auto const &[name, sr] : m_catalog) {
        if (name != CATALOG_NAME) {
            vec.push_back(&sr);
        }
    }
    std::sort(vec.begin(), vec.end(), [](const SectionRecord *a, const SectionRecord *b) {
        return a->offset < b->offset;
    });
    // Here must be `at` for this method is `const`.
    auto offset = m_catalog.at(CATALOG_NAME).len();
    for (auto i : vec) {
        if (i->size == 0) { // Ignore temprary records.
            continue;
        }
        if (i->offset >= offset + size) {
            break;
        }
        offset = i->offset + i->size;
    }
    return offset;
}

std::ostream &operator<<(std::ostream &os, const FileStore &obj)
{
    os << "Catalog of file \"" << obj.m_fileName << "\":" << std::endl;
    for (auto const &[name, sr] : obj.m_catalog) {
        os << sr << std::endl;
    }
    return os;
}
