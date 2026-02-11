#include "Store.h"

Store::~Store()
{
}

void Store::flush()
{
}

void Store::deleteSectionPrefix(const std::string &prefix)
{
    forEachSection([this, prefix](const std::string &name) -> bool {
        if (name.starts_with(prefix)) {
            deleteSection(name);
        }
        return true;
    });
}

void Store::deleteAllSections()
{
    forEachSection([this](const std::string &name) -> bool {
        deleteSection(name);
        return true;
    });
}
