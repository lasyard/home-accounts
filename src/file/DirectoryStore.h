#ifndef _HA_FILE_DIRECTORY_STORE_H_
#define _HA_FILE_DIRECTORY_STORE_H_

#include <filesystem>

#include "Store.h"

class DirectoryStore : public Store
{
public:
    DirectoryStore(const std::string &dirName);
    virtual ~DirectoryStore();

    void create() override;
    void open() override;
    void clear() override;

    void readSection(const std::string &name, std::string &content) override;
    void writeSection(const std::string &name, const std::string &content) override;
    void deleteSection(const std::string &name) override;

    void forEachSection(const std::function<bool(const std::string &)> &callback) const override;

    bool contains(const std::string &name) const override;

private:
    std::filesystem::path m_dirName;
};

#endif /* _HA_FILE_DIRECTORY_STORE_H_ */
