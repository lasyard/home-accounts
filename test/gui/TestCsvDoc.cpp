#include "doctest/doctest.h"

#include <fstream>
#include <sstream>

#include "CsvDoc.h"

TEST_CASE("read")
{
    const enum column_type types[] = {CT_INT, CT_STR, CT_MONEY, CT_DATE, CT_TIME};
    std::fstream file("sample.csv", std::ios::in);
    CHECK(file.is_open());
    CsvDoc doc(5, types);
    CHECK(doc.Read(file));
    CHECK(doc.GetRecordCount() == 3);
    auto *records = doc.GetRecords();
    auto *item = records->first;
    auto *record = get_record(item);
    CHECK(doc.GetRecordValueString(record, 0) == "1");
    CHECK(doc.GetRecordValueString(record, 1) == "Alice");
    CHECK(doc.GetRecordValueString(record, 2) == "100.01");
    CHECK(doc.GetRecordValueString(record, 3) == "2000-01-01");
    CHECK(doc.GetRecordValueString(record, 4) == "12:34:56");
}

TEST_CASE("write")
{
    const enum column_type types[] = {CT_INT, CT_STR, CT_MONEY, CT_DATE, CT_TIME};
    CsvDoc doc(5, types);
    auto *record1 = doc.InsertRecordHead();
    doc.SetRecordValueString(record1, 0, "1");
    doc.SetRecordValueString(record1, 1, "Alice");
    doc.SetRecordValueString(record1, 2, "100");
    doc.SetRecordValueString(record1, 3, "2000-1-1");
    doc.SetRecordValueString(record1, 4, "12:34:56");
    std::ostringstream os;
    CHECK(doc.Write(os));
    CHECK(os.str() == "1,Alice,100.00,2000-01-01,12:34:56\n");
}
