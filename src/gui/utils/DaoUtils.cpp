#include <wx/arrstr.h>
#include <wx/intl.h>

#include "DaoUtils.h"
#include "IntClientData.h"

#include "data/Joint.h"

const wxString Utils::NA = _("N/A");

void Utils::GetStrings(wxArrayString &arrString, const Joint<const char *, int32_t> *joint)
{
    arrString.Empty();
    if (joint != nullptr) {
        joint->forEachTarget([&arrString](const char *const *str) {
            arrString.push_back(*str);
            return true;
        });
    }
}

void Utils::GetIds(wxClientData **clientData, const Joint<const char *, int32_t> *joint)
{
    int i = 0;
    if (joint != nullptr) {
        joint->forEachSource([clientData, &i](const int32_t *p) {
            clientData[i++] = new IntClientData(*p);
            return true;
        });
    }
}
