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
    CHECK(doc.GetRowCount() == 3);
    CHECK(doc.GetValueString(0, 0) == "1");
    CHECK(doc.GetValueString(0, 1) == "Alice");
    CHECK(doc.GetValueString(0, 2) == "100.01");
    CHECK(doc.GetValueString(0, 3) == "2000-01-01");
    CHECK(doc.GetValueString(0, 4) == "12:34:56");
    CHECK(doc.GetValueString(1, 0) == "2");
    CHECK(doc.GetValueString(1, 1) == "Betty");
    CHECK(doc.GetValueString(1, 2) == "200.00");
    CHECK(doc.GetValueString(1, 3) == "2025-12-31");
    CHECK(doc.GetValueString(1, 4) == "");
    CHECK(doc.GetValueString(2, 0) == "3");
    CHECK(doc.GetValueString(2, 1) == "Cindy");
    CHECK(doc.GetValueString(2, 2) == "3.00");
    CHECK(doc.GetValueString(2, 3) == "2039-05-05");
    CHECK(doc.GetValueString(2, 4) == "01:02:03");
}

TEST_CASE("write")
{
    const enum column_type types[] = {CT_INT, CT_STR, CT_MONEY, CT_DATE, CT_TIME};
    CsvDoc doc(5, types);
    doc.AddRecord();
    doc.SetValueString(0, 0, "1");
    doc.SetValueString(0, 1, "Alice");
    doc.SetValueString(0, 2, "100");
    doc.SetValueString(0, 3, "2000-1-1");
    doc.SetValueString(0, 4, "12:34:56");
    std::ostringstream os;
    CHECK(doc.Write(os));
    CHECK(os.str() == "1,Alice,100.00,2000-01-01,12:34:56\n");
}
