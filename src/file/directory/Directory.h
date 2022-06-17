#ifndef _DIRECTORY_DIRECTORY_H_
#define _DIRECTORY_DIRECTORY_H_

#include <filesystem>

#include "../SectionStore.h"

class Directory : public SectionStore
{
public:
    Directory(const std::string &dirName);
    virtual ~Directory();

    void create() override;
    void open() override;
    void clear() override;

    void readSection(const std::string &name, std::ostream &content) override;
    void readSection(const std::string &name, std::string &content) override;
    void writeSection(const std::string &name, std::istream &content) override;
    void writeSection(const std::string &name, const std::string &content) override;
    void deleteSection(const std::string &name) override;

    void getSectionNames(std::vector<std::string> &names) const override;

    bool contains(const std::string &name) const override;
    bool operator==(const SectionStore &obj) const override;

private:
    std::filesystem::path m_dirName;

    static std::streamsize streamCopy(std::istream &in, std::ostream &out);
};

#endif /* _DIRECTORY_DIRECTORY_H_ */
