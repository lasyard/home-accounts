#ifndef _HA_GUI_HA_TABLE_TEMPLATE_H_
#define _HA_GUI_HA_TABLE_TEMPLATE_H_

#include "HaTable.h"

template <typename I, typename DOC> class HaTableTemplate : public HaTable
{
public:
    HaTableTemplate(DOC *doc) : HaTable(doc), m_headerImpls(), m_colImpls()
    {
    }

    virtual ~HaTableTemplate()
    {
    }

    DOC *GetDoc()
    {
        return static_cast<DOC *>(m_doc);
    }

    const DOC *GetDoc() const
    {
        return static_cast<const DOC *>(m_doc);
    }

    int GetNumberCols() override
    {
        return m_colImpls.size();
    }

    enum column_type GetColType(int col) const override
    {
        return col < GetColsCount() ? m_colImpls[col].type : CT_IGNORE;
    }

    bool IsColReadOnly(int col) const override
    {
        return m_colImpls[col].set == nullptr;
    }

    record_t *GetRowRecord(int row) const override
    {
        auto headers = m_headerImpls.size();
        return ((size_t)row >= headers && row < GetRowsCount()) ? m_doc->GetRecord(row - headers) : nullptr;
    }

    wxString GetColLabelValue(int col) override
    {
        return m_colImpls[col].label;
    }

    wxString GetRowLabelValue(int row) override
    {
        if ((size_t)row < m_headerImpls.size()) {
            return m_headerImpls[row].label;
        } else if (row < GetRowsCount()) {
            return wxString::Format("%d", row + 1 - (int)m_headerImpls.size());
        }
        return wxEmptyString;
    }

protected:
    struct CellImpl {
        wxString label;
        enum column_type type;
        wxString (I::*get)(int row, int col) const;
        void (I::*set)(int row, int col, const wxString &value);
        int pos; // save corresponding row/col in the doc
    };

    std::vector<struct CellImpl> m_headerImpls;
    std::vector<struct CellImpl> m_colImpls;

    void SetImpl(
        const wxString &label,
        CellImpl &impl,
        enum column_type type,
        wxString (I::*get)(int row, int col) const = nullptr,
        void (I::*set)(int row, int col, const wxString &value) = nullptr,
        int pos = -1
    )
    {
        impl.label = label;
        impl.type = type;
        impl.get = get;
        impl.set = set;
        impl.pos = pos;
    }

    void SetColImpl(
        const wxString &label,
        int dst,
        enum column_type type,
        wxString (I::*get)(int row, int col) const = nullptr,
        void (I::*set)(int row, int col, const wxString &value) = nullptr,
        int pos = -1
    )
    {
        SetImpl(label, m_colImpls[dst], type, get, set, pos);
    }

    void SetColImplDoc(const wxString &label, int dst, int col, bool ro = false)
    {
        SetColImpl(label, dst, m_doc->GetColType(col), &I::DocGetter, !ro ? &I::DocSetter : nullptr, col);
    }

    void SetHeaderImpl(
        const wxString &label,
        int row,
        enum column_type type,
        wxString (I::*get)(int row, int col) const = nullptr,
        void (I::*set)(int row, int col, const wxString &value) = nullptr
    )
    {
        SetImpl(label, m_headerImpls[row], type, get, set, row);
    }

    const wxString GetCellValue(int row, int col) const override
    {
        auto headers = m_headerImpls.size();
        if ((size_t)row < headers) {
            auto &impl = m_headerImpls[row];
            if (impl.get != nullptr) {
                return (static_cast<const I *>(this)->*impl.get)(impl.pos, col);
            }
        } else {
            auto flag = GetRowRecordFlag(row);
            if (flag == RECORD_FLAG_COMMENT) {
                if (col == 0) {
                    return GetCommentString(row);
                }
            } else if (col < GetColsCount()) {
                auto &impl = m_colImpls[col];
                if (impl.get != nullptr) {
                    return (static_cast<const I *>(this)->*impl.get)(row - headers, impl.pos);
                }
                return _("not implemented");
            }
        }
        return wxEmptyString;
    }

    void SetCellValue(int row, int col, const wxString &value) override
    {
        auto headers = m_headerImpls.size();
        if ((size_t)row < headers) {
            auto &impl = m_headerImpls[row];
            if (impl.set != nullptr) {
                (static_cast<I *>(this)->*impl.set)(impl.pos, col, value);
            }
        } else {
            wxASSERT(GetRowRecordFlag(row) == RECORD_FLAG_NORMAL);
            if (col < GetColsCount() && m_colImpls[col].set != nullptr) {
                auto &impl = m_colImpls[col];
                (static_cast<I *>(this)->*impl.set)(row - headers, impl.pos, value);
            }
        }
    }
};

#endif /* _HA_GUI_HA_TABLE_TEMPLATE_H_ */
