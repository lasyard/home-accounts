#include <wx/arrstr.h>

#include "DaoUtils.h"

#include "data/Joint.h"

void Utils::GetStrings(wxArrayString &choices, const Joint<const char *, int32_t> *joint)
{
    choices.Empty();
    if (joint != nullptr) {
        joint->forEach([&choices](const char *const *str) {
            choices.push_back(*str);
            return true;
        });
    }
}
