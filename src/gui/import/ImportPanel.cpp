#include <fstream>

#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/sizer.h>

#include "ImportDoc.h"
#include "ImportGrid.h"
#include "ImportPanel.h"

#include "../HaDocument.h"

#include "file/Exceptions.h"

IMPLEMENT_TM(ImportPanel)
IMPLEMENT_DYNAMIC_CLASS(ImportPanel, HaPanel)

BEGIN_EVENT_TABLE(ImportPanel, HaPanel)
END_EVENT_TABLE()

const char *const ImportPanel::IMPORT_SECTION_NAME = "import";

ImportPanel::ImportPanel(wxWindow *parent) : HaPanel(parent)
{
    wxLog::AddTraceMask(TM);
    m_grid = Utils::AddSoleGrid<ImportGrid>(this);
}

ImportPanel::~ImportPanel()
{
}

void ImportPanel::OnUpdate()
{
    auto *csv = new ImportDoc();
    try {
        auto &data = m_doc->GetSection(ImportPanel::IMPORT_SECTION_NAME);
        m_ok = csv->Read(data);
    } catch ([[maybe_unused]] SectionNotFound &e) {
        wxFileDialog dlg(
            nullptr,
            _("Select CSV file to import"),
            wxEmptyString,
            wxEmptyString,
            _("CSV files (*.csv)|*.csv|All files (*.*)|*.*"),
            wxFD_OPEN | wxFD_FILE_MUST_EXIST
        );
        if (dlg.ShowModal() != wxID_OK) {
            delete csv;
            return;
        }
        wxString path = dlg.GetPath();
        std::ifstream ifs(w2s(path));
        if (!ifs.is_open()) {
            wxLogError(_("Failed to open file: %s"), path);
            delete csv;
            return;
        }
        m_ok = csv->ReadStream(ifs);
        // there may be something read, even it is not ok
        std::string str;
        csv->Write(str);
        m_doc->SaveOrDeleteSection(ImportPanel::IMPORT_SECTION_NAME, str);
        m_doc->Modify(true);
    }
    m_grid->InitTable(csv);
}

void ImportPanel::SaveContents()
{
    // do nothing, as the contents is read only
}
