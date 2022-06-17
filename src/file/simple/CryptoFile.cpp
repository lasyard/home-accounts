#include <cryptopp/filters.h>
#include <sstream>

#include "../FileExeptions.h"
#include "CryptoFile.h"

const char *const CryptoFile::CATALOG_NAME = "__catalog__";

CryptoFile::CryptoFile(const std::string &fileName, const std::string &pass, const std::string &iv)
    : CryptedSectionStore(), m_fileName(fileName), m_file(), m_catalog()
{
    srand(time(NULL));
    setIV(m_iv, iv);
    getKey(m_key, pass);
}

CryptoFile::~CryptoFile()
{
    if (m_file.is_open()) {
        flush();
        m_file.close();
    }
}

void CryptoFile::create()
{
    m_file.open(m_fileName, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    if (!m_file.is_open()) {
        throw FileOpen(m_fileName);
    }
    clear();
}

void CryptoFile::open()
{
    m_file.open(m_fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!m_file.is_open()) {
        throw FileOpen(m_fileName);
    }
    loadCatalog();
}

void CryptoFile::clear()
{
    // Clear all sections.
    m_catalog.clear();
    // Construct new catalog.
    CatalogRecord cr;
    // Must not be empty, or it is treated as a new section.
    cr.name = CATALOG_NAME;
    memcpy(cr.key, m_key, CRYPTO_KEY_LEN);
    m_catalog[CATALOG_NAME] = cr;
}

void CryptoFile::flush()
{
    saveCatalog();
    m_file.flush();
}

void CryptoFile::deleteSection(const std::string &name)
{
    m_catalog.erase(name);
}

void CryptoFile::forEachSection(std::function<bool(const std::string &name)> fun)
{
    for (auto &[name, c] : m_catalog) {
        if (name != CATALOG_NAME) {
            if (!fun(name)) {
                break;
            }
        }
    }
}

bool CryptoFile::contains(const std::string &name) const
{
    return m_catalog.find(name) != m_catalog.end();
}

bool CryptoFile::operator==(const SectionStore &obj) const
{
    try {
        const CryptoFile &file = dynamic_cast<const CryptoFile &>(obj);
        return file.m_fileName == m_fileName;
    } catch (std::bad_cast &e) {
        return false;
    }
}

void CryptoFile::changePass(const std::string &pass)
{
    getKey(m_key, pass);
}

void CryptoFile::decryptSection(const std::string &name, std::string &content)
{
    auto &cr = m_catalog.at(name);
    size_t size = cr.size;
    byte *buf = new byte[size];
    m_file.seekg(cr.offset);
    m_file.read((char *)buf, size);
    if ((size_t)m_file.gcount() < size) {
        throw FileCorrupt("Section is too short.");
    }
    try {
        decrypt(buf, size, content, cr.key, m_iv);
    } catch (CryptoPP::HashVerificationFilter::HashVerificationFailed &) {
        delete[] buf;
        throw BadPassword();
    }
    delete[] buf;
}

void CryptoFile::encryptSection(const std::string &name, const std::string &content)
{
    auto &cr = m_catalog[name];
    if (cr.name.empty()) { // New created section.
        cr.name = name;
        newKey(cr.key);
    }
    std::string output;
    encrypt(content, output, cr.key, m_iv);
    size_t size = output.length();
    off_t offset = findSlot(size);
    cr.offset = offset;
    cr.size = size;
    m_file.seekp(offset);
    m_file.write(output.c_str(), size);
}

void CryptoFile::loadCatalog()
{
    m_catalog.clear();
    m_file.seekg(0);
    // Read catalog info.
    CatalogRecord cr;
    if (!cr.read(m_file)) {
        throw FileCorrupt("CATALOG is too short.");
    }
    memcpy(cr.key, m_key, CRYPTO_KEY_LEN);
    m_catalog[CATALOG_NAME] = cr;
    // Read catalog.
    std::stringstream content;
    readSection(CATALOG_NAME, content);
    // Read info of sections.
    while (cr.read(content)) {
        m_catalog[cr.name] = cr;
    }
}

void CryptoFile::saveCatalog()
{
    std::stringstream content;
    for (auto &[name, cr] : m_catalog) {
        if (name == CATALOG_NAME) {
            memcpy(cr.key, m_key, CRYPTO_KEY_LEN);
            continue;
        }
        cr.write(content);
    }
    writeSection(CATALOG_NAME, content);
    CatalogRecord cr = m_catalog[CATALOG_NAME];
    // Don't write pass into file.
    memcpy(cr.key, "NO PASSWORD HERE", CRYPTO_KEY_LEN);
    m_file.seekp(0);
    cr.write(m_file);
}

off_t CryptoFile::findSlot(size_t size) const
{
    std::vector<const CatalogRecord *> vec;
    // Must reference to get the real address in `m_catalog`.
    for (auto const &[name, cr] : m_catalog) {
        if (name != CATALOG_NAME) {
            vec.push_back(&cr);
        }
    }
    std::sort(vec.begin(), vec.end(), [](const CatalogRecord *a, const CatalogRecord *b) {
        return a->offset < b->offset;
    });
    // Here must be `at` for this method is `const`.
    off_t offset = (off_t)m_catalog.at(CATALOG_NAME).len();
    for (auto i : vec) {
        if (i->size == 0) { // Ignore temprary records.
            continue;
        }
        if (i->offset >= offset + (off_t)size) {
            break;
        }
        offset = i->offset + i->size;
    }
    return offset;
}

std::ostream &operator<<(std::ostream &os, const CryptoFile &obj)
{
    os << "Catalog of file \"" << obj.m_fileName << "\":" << std::endl;
    for (auto const &[name, cr] : obj.m_catalog) {
        os << cr << std::endl;
    }
    return os;
}
