#include <fstream>
#include <sstream>

#include "../FileExeptions.h"
#include "Directory.h"

#define BUF_LEN 1024

namespace fs = std::filesystem;

Directory::Directory(const std::string &dirName) : SectionStore(), m_dirName(dirName)
{
}

Directory::~Directory()
{
}

void Directory::create()
{
    if (!fs::is_directory(m_dirName)) {
        fs::create_directories(m_dirName);
    } else {
        throw DirCreate(m_dirName);
    }
}

void Directory::open()
{
}

void Directory::clear()
{
    for (auto const &entry : fs::directory_iterator(m_dirName)) {
        fs::remove_all(entry);
    }
}

void Directory::readSection(const std::string &name, std::ostream &content)
{
    std::ifstream file(m_dirName / name, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        throw SectionNotFound(name);
    }
    streamCopy(file, content);
    file.close();
}

void Directory::readSection(const std::string &name, std::string &content)
{
    std::stringstream str;
    readSection(name, str);
    content = str.str();
}

void Directory::writeSection(const std::string &name, std::istream &content)
{
    auto path = m_dirName / name;
    if (!fs::is_directory(path.parent_path())) {
        fs::create_directories(path.parent_path());
    }
    std::ofstream file(m_dirName / name, std::ios::binary | std::ios::out | std::ios::trunc);
    streamCopy(content, file);
    file.close();
}

void Directory::writeSection(const std::string &name, const std::string &content)
{
    std::stringstream str;
    str << content;
    writeSection(name, str);
}

void Directory::deleteSection(const std::string &name)
{
    auto path = m_dirName / name;
    do {
        fs::remove(path);
        path = path.parent_path();
    } while (path != m_dirName && fs::is_empty(path));
}

void Directory::getSectionNames(std::vector<const std::string> &names) const
{
    for (auto const &entry : fs::recursive_directory_iterator(m_dirName)) {
        names.push_back(entry.path().string().substr(m_dirName.string().length()));
    }
}

bool Directory::contains(const std::string &name) const
{
    return is_regular_file(m_dirName / name);
}

bool Directory::operator==(const SectionStore &obj) const
{
    try {
        const Directory &dir = dynamic_cast<const Directory &>(obj);
        return dir.m_dirName == m_dirName;
    } catch (std::bad_cast &e) {
        return false;
    }
}

std::streamsize Directory::streamCopy(std::istream &in, std::ostream &out)
{
    char *buf = new char[BUF_LEN];
    int count = 0;
    while (!in.eof()) {
        in.read(buf, BUF_LEN);
        out.write(buf, in.gcount());
        count += in.gcount();
    }
    delete[] buf;
    return count;
}
