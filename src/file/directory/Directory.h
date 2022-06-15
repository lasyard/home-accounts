#ifndef _DIRECTORY_DIRECTORY_H_
#define _DIRECTORY_DIRECTORY_H_

#include <filesystem>

#include "../SectionStore.h"

class Directory : public SectionStore
{
public:
    Directory(const std::string &dirName);
    virtual ~Directory();

    Directory *create() override;
    Directory *open() override;
    Directory *readSection(const std::string &name, std::ostream &content) override;
    Directory *readSection(const std::string &name, std::string &content) override;
    Directory *writeSection(const std::string &name, std::istream &content) override;
    Directory *writeSection(const std::string &name, const std::string &content) override;
    Directory *deleteSection(const std::string &name) override;
    Directory *clear() override;
    Directory *flush() override;

    bool contains(const std::string &name) const override;
    bool operator==(const SectionStore &obj) const override;

    void forEachSection(std::function<bool(const std::string &name)> fun) override;

private:
    std::filesystem::path m_dirName;

    static std::streamsize streamCopy(std::istream &in, std::ostream &out);
};

#endif /* _DIRECTORY_DIRECTORY_H_ */
