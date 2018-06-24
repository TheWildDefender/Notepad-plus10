#include <wx/wx.h>
#include "editor.hpp"

IMPLEMENT_APP(EditorApp)

bool EditorApp::OnInit() { //create window
	EditorFrame *EditorWindow = new EditorFrame (_("Notepad++++++++++ - untitled"), wxDefaultPosition, wxSize(1000, 500));
	EditorWindow->Show(true);
	SetTopWindow(EditorWindow);
	return true;
}

BEGIN_EVENT_TABLE(EditorFrame, wxFrame) //event handlers
	EVT_MENU(MENUITEM_Exit, EditorFrame::Exit)
	EVT_MENU(MENUITEM_Save, EditorFrame::Save)
	EVT_MENU(MENUITEM_SaveAs, EditorFrame::SaveAs)
	EVT_MENU(MENUITEM_Open, EditorFrame::Open)
	EVT_MENU(MENUITEM_Clear, EditorFrame::Clear)
	EVT_TEXT(TEXTCTRL_Main, EditorFrame::ChangeToUnsaved)
	EVT_CLOSE(EditorFrame::Exit)
END_EVENT_TABLE()

EditorFrame::EditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame (NULL, -1, title, pos, size) { //initialize components
	TextBox = new wxTextCtrl (this, TEXTCTRL_Main, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
	MenuBar = new wxMenuBar ();
	wxMenu *FileMenu = new wxMenu ();
	ConfirmExitDialog = new wxMessageDialog (this, _("Do you want to save your unsaved changes?"), _("Unsaved Changes"), wxYES_NO | wxCANCEL | wxICON_EXCLAMATION | wxCENTRE);
	OpenFileDialog = new wxFileDialog (this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWi/*is this a bug, or an easter egg?*/ldcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	SaveFileDialog = new wxFileDialog (this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	//initialize menu items
	ExitMenuItem = new wxMenuItem (FileMenu, MENUITEM_Exit, _("Exit\tCtrl-Q"), wxEmptyString);
	SaveMenuItem = new wxMenuItem (FileMenu, MENUITEM_Save, _("Save\tCtrl-S"), wxEmptyString);
	SaveAsMenuItem = new wxMenuItem (FileMenu, MENUITEM_SaveAs, _("Save As"), wxEmptyString);
	OpenMenuItem = new wxMenuItem (FileMenu, MENUITEM_Open, _("Open\tCtrl-O"), wxEmptyString);
	ClearMenuItem = new wxMenuItem (FileMenu, MENUITEM_Clear, _("Clear\tCtrl-Del"), wxEmptyString);

	//set icons for menu items
	ExitMenuItem->SetBitmap(wxBitmap ("icons/quit.xpm", wxBITMAP_TYPE_XPM));
	SaveMenuItem->SetBitmap(wxBitmap ("icons/filesave.xpm", wxBITMAP_TYPE_XPM));
	SaveAsMenuItem->SetBitmap(wxBitmap ("icons/filesaveas.xpm", wxBITMAP_TYPE_XPM));
	OpenMenuItem->SetBitmap(wxBitmap ("icons/fileopen.xpm", wxBITMAP_TYPE_XPM));
	ClearMenuItem->SetBitmap(wxBitmap ("icons/delete.xpm", wxBITMAP_TYPE_XPM));

	//add menu items to menu
	FileMenu->Append(ExitMenuItem);
	FileMenu->Append(SaveMenuItem);
	FileMenu->Append(SaveAsMenuItem);
	FileMenu->Append(OpenMenuItem);
	FileMenu->Append(ClearMenuItem);

	MenuBar->Append(FileMenu, _("File"));
	SetMenuBar(MenuBar);
}

void EditorFrame::Exit(wxCommandEvent& event) { //exit function for exit menu item
	if (ConfirmExit() == true) Destroy();
}

void EditorFrame::Exit(wxCloseEvent& event) { //exit function for window close events
	if (ConfirmExit() == true) Destroy();
}

void EditorFrame::Save(wxCommandEvent& event) {
	if (opened_file_name == wxString ("untitled")) SaveAs(null_event);
	else TextBox->SaveFile(opened_file_path);
	saved = true;
}

void EditorFrame::SaveAs(wxCommandEvent& event) {
	dialog_input = SaveFileDialog->ShowModal(); //event handlers must be of type void :-(
	if (dialog_input == wxID_OK) {
		opened_file_path = SaveFileDialog->GetPath();
		opened_file_name = SaveFileDialog->GetFilename();
		TextBox->SaveFile(opened_file_path);
		this->SetTitle(wxString ("Notepad++++++++++ - ") + opened_file_name);
		saved = true;
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

bool EditorFrame::ConfirmExit() { //returns true if user wants to exit, and returns false if user wants to stay
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
