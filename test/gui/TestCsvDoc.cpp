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
    CHECK(doc.GetRecordValueString(0, 0) == "1");
    CHECK(doc.GetRecordValueString(0, 1) == "Alice");
    CHECK(doc.GetRecordValueString(0, 2) == "100.01");
    CHECK(doc.GetRecordValueString(0, 3) == "2000-01-01");
    CHECK(doc.GetRecordValueString(0, 4) == "12:34:56");
    CHECK(doc.GetRecordValueString(1, 0) == "2");
    CHECK(doc.GetRecordValueString(1, 1) == "Betty");
    CHECK(doc.GetRecordValueString(1, 2) == "200.00");
    CHECK(doc.GetRecordValueString(1, 3) == "2025-12-31");
    CHECK(doc.GetRecordValueString(1, 4) == "");
    CHECK(doc.GetRecordValueString(2, 0) == "3");
    CHECK(doc.GetRecordValueString(2, 1) == "Cindy");
    CHECK(doc.GetRecordValueString(2, 2) == "3.00");
    CHECK(doc.GetRecordValueString(2, 3) == "2039-05-05");
    CHECK(doc.GetRecordValueString(2, 4) == "01:02:03");
}

TEST_CASE("write")
{
    const enum column_type types[] = {CT_INT, CT_STR, CT_MONEY, CT_DATE, CT_TIME};
    CsvDoc doc(5, types);
    doc.AddRecord();
    doc.SetRecordValueString(0, 0, "1");
    doc.SetRecordValueString(0, 1, "Alice");
    doc.SetRecordValueString(0, 2, "100");
    doc.SetRecordValueString(0, 3, "2000-1-1");
    doc.SetRecordValueString(0, 4, "12:34:56");
    std::ostringstream os;
    CHECK(doc.Write(os));
    CHECK(os.str() == "1,Alice,100.00,2000-01-01,12:34:56\n");
}
