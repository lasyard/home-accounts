#ifndef _HA_GUI_HA_TABLE_INDEX_H_
#define _HA_GUI_HA_TABLE_INDEX_H_

class HaTableIndex
{
public:
    enum Type { SEGMENT, ITEM, INVALID };

    explicit HaTableIndex(struct segment *segment) : m_segment(segment), m_item(nullptr), m_seq(0)
    {
    }

    explicit HaTableIndex(struct segment *segment, void *item, int seq) : m_segment(segment), m_item(item), m_seq(seq)
    {
    }

    enum Type GetType() const
    {
        if (m_item == nullptr) {
            return SEGMENT;
        }
        return ITEM;
    }

    const int &GetSeq() const
    {
        return m_seq;
    }

    int &GetSeq()
    {
        return m_seq;
    }

    struct segment *GetSegment() const
    {
        return m_segment;
    }

    void *GetItem() const
    {
        return m_item;
    }

private:
    struct segment *m_segment;
    void *m_item;
    int m_seq;
};

#endif /* _HA_GUI_HA_TABLE_INDEX_H_ */
