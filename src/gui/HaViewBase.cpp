#include <wx/listbook.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>

#include "ChangePassDialog.h"
#include "HaDocumentBase.h"
#include "HaViewBase.h"

IMPLEMENT_TM(HaViewBase)

bool HaViewBase::OnClose(bool deleteWindow)
{
    if (!wxView::OnClose(deleteWindow)) {
        return false;
    }
    Activate(false);
    // Not cleared defaultly, so clear it.
    static_cast<wxFrame *>(GetFrame())->SetTitle("");
    return true;
}

void HaViewBase::OnDraw([[maybe_unused]] wxDC *dc)
{
    // Do nothing.
}

void HaViewBase::OnChangePass([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto doc = GetHaDocument();
    ChangePassDialog dlg(nullptr, doc->GetPass());
    if (dlg.ShowModal() == wxID_OK) {
        doc->ChangePass(dlg.GetPass());
    }
}

HaDocumentBase *HaViewBase::GetHaDocument() const
{
    auto doc = GetDocument();
    return static_cast<HaDocumentBase *>(doc);
}
