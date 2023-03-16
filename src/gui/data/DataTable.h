#ifndef _DATA_DATA_TABLE_H_
#define _DATA_DATA_TABLE_H_

#include "../CachedTable.h"
#include "../CellAttrs.h"

class DataDao;

class DataTable : public CachedTable
{
public:
    static const wxString COL_LABELS[];

    static const int TIME_COL = 0;
    static const int INCOME_COL = 1;
    static const int OUTLAY_COL = 2;
    static const int ACCOUNT_COL = 3;
    static const int CHANNEL_COL = 4;
    static const int DESC_COL = 5;
    static const int VALID_COL = 6;

    static const size_t COL_NUM = 7;

    DataTable(DataDao *dataDao);
    virtual ~DataTable();

    wxString GetRowLabelValue(int row) override;

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) override;

    void SetAccountChoices(wxArrayString &choices);
    void SetChannelChoices(wxArrayString &choices);

private:
    DataDao *m_dataDao;
    wxGridCellAttr *m_pageTitleAttr;
    wxGridCellAttr *m_accountAttr;
    wxGridCellAttr *m_channelAttr;

    wxString GetCellValue(int row, int col) override;

    void SetCellValue(int row, int col, const std::string &value) override;

    bool InsertRow(size_t pos) override;
    bool AppendRow() override;

    bool DeleteRow([[maybe_unused]] size_t pos) override
    {
        // Do nothing, deletion is done in `DataGrid`.
        return true;
    }

    wxGridCellAttr *GetPageTitleAttr()
    {
        m_pageTitleAttr->IncRef();
        return m_pageTitleAttr;
    }

    wxGridCellAttr *GetAccountAttr()
    {
        m_accountAttr->IncRef();
        return m_accountAttr;
    }

    wxGridCellAttr *GetChannelAttr()
    {
        m_channelAttr->IncRef();
        return m_channelAttr;
    }

    void SetChoicesOf(wxGridCellAttr *&attr, const wxArrayString &choices)
    {
        if (attr->IsReadOnly()) {
            attr->DecRef();
            attr = CellAttrs::ins().CloneDefault();
        }
        attr->SetEditor(new wxGridCellChoiceEditor(choices));
    }
};

#endif /* _DATA_DATA_TABLE_H_ */
