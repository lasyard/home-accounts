#ifndef _UTILS_DAO_UTILS_H_
#define _UTILS_DAO_UTILS_H_

#include <cstdint>

class wxArrayString;

template <typename T, typename S> class Joint;

namespace Utils
{

void GetStrings(wxArrayString &choices, const Joint<const char *, int32_t> *joint);

} // namespace Utils

#endif /* _UTILS_DAO_UTILS_H_ */
