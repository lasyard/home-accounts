#ifndef _DATA_CSV_ROW_TRAITS_H_
#define _DATA_CSV_ROW_TRAITS_H_

#include "csv/ColumnType.h"

template <typename I> class CsvRowTraits
{
public:
    static const int cols;
    static const ColumnType types[];

    static void *readPtr(void *data, int i);
    static const void *writePtr(const void *data, int i);
};

#endif /* _DATA_CSV_ROW_TRAITS_H_ */
