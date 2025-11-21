#include <fstream>

#include "DirectoryStore.h"
#include "Exceptions.h"

namespace fs = std::filesystem;

DirectoryStore::DirectoryStore(const std::string &dirName) : Store(), m_dirName(dirName)
{
}

DirectoryStore::~DirectoryStore()
{
}

void DirectoryStore::create()
{
    if (!fs::is_directory(m_dirName)) {
        fs::create_directories(m_dirName);
    } else {
        throw FileOpen(m_dirName.string());
    }
}

void DirectoryStore::open()
{
}

void DirectoryStore::clear()
{
    for (auto const &entry : fs::directory_iterator(m_dirName)) {
        fs::remove_all(entry);
    }
}

void DirectoryStore::readSection(const std::string &name, std::string &content)
{
    content.clear();
    std::ifstream file(m_dirName / name, std::ios::in | std::ios::binary);
    if (!file) {
        return;
    }
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    content.resize(static_cast<size_t>(size));
    if (size > 0 && !file.read(&content[0], size)) {
        throw std::runtime_error("failed to read file: " + name);
    }
}

void DirectoryStore::writeSection(const std::string &name, const std::string &content)
{
    auto path = m_dirName / name;
    if (!fs::is_directory(path.parent_path())) {
        fs::create_directories(path.parent_path());
    }
    std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!file) {
        throw std::runtime_error("failed to write file: " + name);
    }
    file << content;
    file.close();
}

void DirectoryStore::deleteSection(const std::string &name)
{
    auto path = m_dirName / name;
    do {
        fs::remove(path);
        path = path.parent_path();
    } while (path != m_dirName && fs::is_empty(path));
}

void DirectoryStore::forEachSection(std::function<bool(const std::string &)> callback) const
{
    for (auto const &entry : fs::recursive_directory_iterator(m_dirName)) {
        if (!callback(entry.path().string().substr(m_dirName.string().length()))) {
            break;
        };
    }
}

bool DirectoryStore::contains(const std::string &name) const
{
    return is_regular_file(m_dirName / name);
}

bool DirectoryStore::operator==(const Store &obj) const
{
    try {
        const DirectoryStore &dir = dynamic_cast<const DirectoryStore &>(obj);
        return dir.m_dirName == m_dirName;
    } catch (std::bad_cast &) {
        return false;
    }
}
