#ifndef _DATA_TYPE_TRAITS_H_
#define _DATA_TYPE_TRAITS_H_

template <typename T> class TypeTraits
{
public:
    static T zero();
    static int compare(T a, T b);
};

#endif /* _DATA_TYPE_TRAITS_H_ */
