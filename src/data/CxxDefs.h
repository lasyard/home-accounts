#ifndef _DATA_CXX_DEFS_H_
#define _DATA_CXX_DEFS_H_

template <typename T> void safe_delete(T *&ptr)
{
    if (ptr != nullptr) {
        delete ptr;
        ptr = nullptr;
    }
}

#endif /* _DATA_CXX_DEFS_H_ */
