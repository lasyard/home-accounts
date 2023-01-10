#include <wx/listbook.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>

#include "ChangePassDialog.h"
#include "HaDocumentBase.h"
#include "HaViewBase.h"

IMPLEMENT_TM(HaViewBase)

bool HaViewBase::OnClose(bool deleteWindow)
{
    Activate(false);
    // Not cleared defaultly, so clear it.
    static_cast<wxFrame *>(GetFrame())->SetTitle("");
    ClearContents();
    return wxView::OnClose(deleteWindow);
}

void HaViewBase::OnDraw([[maybe_unused]] wxDC *dc)
{
    // Do nothing.
}

void HaViewBase::OnClosingDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
}

void HaViewBase::OnChangePass([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto doc = static_cast<HaDocumentBase *>(GetDocument());
    ChangePassDialog dlg(nullptr, doc->GetPass());
    if (dlg.ShowModal() == wxID_OK) {
        doc->ChangePass(dlg.GetPass());
    }
}
