#ifndef _HA_GUI_HA_CSV_TEMPLATE_H_
#define _HA_GUI_HA_CSV_TEMPLATE_H_

#include <wx/log.h>

#include "HaCsv.h"

#include "csv/str.h"

template <typename I> class HaCsvTemplate : public HaCsv
{
public:
    HaCsvTemplate() : HaCsv(), m_accessors()
    {
    }

    virtual ~HaCsvTemplate()
    {
    }

    enum column_type GetColType(int i) const override
    {
        wxASSERT(i < GetColCount());
        return m_accessors[i].type;
    }

    const wxString GetValueString(int pos, int i) const override
    {
        record_t *record = GetRecord(pos);
        wxASSERT(record != nullptr && is_index_valid(&m_parser, record, i));
        auto &accessor = m_accessors[i];
        if (accessor.get != nullptr) {
            return (static_cast<const I *>(this)->*accessor.get)(record, i);
        }
        return wxEmptyString;
    }

    void SetValueString(int pos, int i, const wxString &value) override
    {
        record_t *record = GetRecord(pos);
        wxASSERT(record != nullptr && is_index_valid(&m_parser, record, i));
        auto &accessor = m_accessors[i];
        if (accessor.set != nullptr) {
            (static_cast<I *>(this)->*accessor.set)(record, i, value);
        }
    }

    void SetParser(int cols, const enum column_type types[], int comment_cols) override
    {
        HaCsv::SetParser(cols, types, comment_cols);
        m_accessors.resize(cols);
        for (int i = 0; i < cols; ++i) {
            if (m_parser.meta->types[i] != CT_STR) {
                SetAccessor(i, types[i], &I::DefaultGetter, &I::DefaultSetter);
            } else {
                SetAccessor(i, types[i], &I::StrGetter, &I::DefaultSetter);
            }
        }
    }

protected:
    struct Accessor {
        enum column_type type;
        const wxString (I::*get)(const record_t *record, int i) const;
        void (I::*set)(record_t *record, int i, const wxString &value);
    };

    std::vector<Accessor> m_accessors;

    void SetAccessor(
        int col,
        enum column_type type,
        const wxString (I::*get)(const record_t *record, int i) const,
        void (I::*set)(record_t *record, int i, const wxString &value)
    )
    {
        m_accessors[col].type = type;
        m_accessors[col].get = get;
        m_accessors[col].set = set;
    }

    const wxString DefaultGetter(const record_t *record, int i) const
    {
        char buf[MAX_LINE_LENGTH + 1];
        char *p = output_field(&m_parser, buf, record, i);
        *p = '\0';
        return wxString(buf);
    }

    const wxString StrGetter(const record_t *record, int i) const
    {
        auto *s = (struct str *)get_const_field(&m_parser, record, i);
        if (!str_is_empty(s)) {
            return wxString(s->buf, s->len);
        }
        return wxEmptyString;
    }

    void DefaultSetter(record_t *record, int i, const wxString &value)
    {
        if (parse_field(&m_parser, value.c_str(), record, i) == NULL) {
            wxLogError(_("Invalid value: %s"), value);
        }
    }
};

#endif /* _HA_GUI_HA_CSV_TEMPLATE_H_ */
