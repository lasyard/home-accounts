#ifndef _GUI_INT_CLIENT_DATA_H_
#define _GUI_INT_CLIENT_DATA_H_

#include <wx/clntdata.h>

class IntClientData : public wxClientData
{
public:
    IntClientData(int data) : m_data(data)
    {
    }

    virtual ~IntClientData()
    {
    }

    int get() const
    {
        return m_data;
    }

private:
    int m_data;
};

#endif /* _GUI_INT_CLIENT_DATA_H_ */
