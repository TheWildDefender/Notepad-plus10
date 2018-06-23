#include <wx/wx.h>
#include "editor.hpp"

IMPLEMENT_APP(EditorApp)

bool EditorApp::OnInit() {
	EditorFrame *EditorWindow = new EditorFrame (_("Notepad++++++++++ - untitled"), wxDefaultPosition, wxSize(1000, 500));
	EditorWindow->Show(true);
	SetTopWindow(EditorWindow);
	return true;
}

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)
	EVT_MENU(FILEMENU_Exit, EditorFrame::Exit)
	EVT_MENU(FILEMENU_Save, EditorFrame::Save)
	EVT_MENU(FILEMENU_SaveAs, EditorFrame::SaveAs)
	EVT_MENU(FILEMENU_Open, EditorFrame::Open)
	EVT_MENU(FILEMENU_Clear, EditorFrame::Clear)
	EVT_TEXT(TEXTCTRL_Main, EditorFrame::ChangeToUnsaved)
	EVT_CLOSE(EditorFrame::Exit)
END_EVENT_TABLE()

EditorFrame::EditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame (NULL, -1, title, pos, size) {
	TextBox = new wxTextCtrl (this, TEXTCTRL_Main, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
	MenuBar = new wxMenuBar ();
	wxMenu *FileMenu = new wxMenu ();
	ConfirmExitDialog = new wxMessageDialog (this, _("Do you want to save your unsaved changes?"), _("Unsaved Changes"), wxYES_NO | wxCANCEL | wxICON_EXCLAMATION | wxCENTRE);
	OpenFileDialog = new wxFileDialog (this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	SaveFileDialog = new wxFileDialog (this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	FileMenu->Append(FILEMENU_Exit, _("Exit"));
	FileMenu->Append(FILEMENU_Save, _("Save"));
	FileMenu->Append(FILEMENU_SaveAs, _("Save As"));
	FileMenu->Append(FILEMENU_Open, _("Open"));
	FileMenu->Append(FILEMENU_Clear, _("Clear"));

	MenuBar->Append(FileMenu, _("File"));
	SetMenuBar(MenuBar);
}

void EditorFrame::Exit(wxCommandEvent& event) {
	if (ConfirmExit() == true) Destroy();
}

void EditorFrame::Exit(wxCloseEvent& event) {
	if (ConfirmExit() == true) Destroy();
}

void EditorFrame::Save(wxCommandEvent& event) {
	if (opened_file_name == wxString ("untitled")) SaveAs(null_event);
	else TextBox->SaveFile(opened_file_path);
	saved = true;
}

void EditorFrame::SaveAs(wxCommandEvent& event) {
	int dialog_input = SaveFileDialog->ShowModal();
	if (dialog_input == wxID_OK) {
		opened_file_path = SaveFileDialog->GetPath();
		opened_file_name = SaveFileDialog->GetFilename();
		TextBox->SaveFile(opened_file_path);
		this->SetTitle(wxString ("Notepad++++++++++ - ") + opened_file_name);
	}
}

void EditorFrame::Open(wxCommandEvent& event) {
	if (OpenFileDialog->ShowModal() == wxID_OK) {
		opened_file_path = OpenFileDialog->GetPath();
		opened_file_name = OpenFileDialog->GetFilename();
		TextBox->LoadFile(opened_file_path);
		this->SetTitle(wxString ("Notepad++++++++++ - ") + opened_file_name);
	}
}

void EditorFrame::Clear(wxCommandEvent& event) {
	TextBox->Clear();
}

void EditorFrame::ChangeToUnsaved(wxCommandEvent& event) {
	saved = false;
}

bool EditorFrame::ConfirmExit() {
	if (saved == false) {
		switch (ConfirmExitDialog->ShowModal()) {
			case wxID_YES:
				SaveAs(null_event);
				if (dialog_input == wxID_OK) return true;
				else return false;
				break;
			case wxID_NO:
				return true;
				break;
			case wxID_CANCEL:
				return false;
				break;
		}
	} else return true;
}
