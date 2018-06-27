#include <wx/wx.h>
#include <wx/hyperlink.h>
#include <random>
#include <chrono>
#include "editor.hpp"

IMPLEMENT_APP(EditorApp)

bool EditorApp::OnInit() { //create window
	EditorFrame *EditorWindow = new EditorFrame (_("Notepad++++++++++ - untitled"), wxDefaultPosition, wxSize(1000, 500));
	EditorWindow->Show(true);
	SetTopWindow(EditorWindow);

	EditorWindow->AboutDialog = new EditorFrame::AboutDialogClass (EditorWindow, _("About"), wxSize (300, 300));

	return true;
}

BEGIN_EVENT_TABLE(EditorFrame, wxFrame) //event handlers for main window
	EVT_MENU(MENUITEM_Exit, EditorFrame::Exit)
	EVT_MENU(MENUITEM_Save, EditorFrame::Save)
	EVT_MENU(MENUITEM_SaveAs, EditorFrame::SaveAs)
	EVT_MENU(MENUITEM_Open, EditorFrame::Open)
	EVT_MENU(MENUITEM_Clear, EditorFrame::Clear)
	EVT_MENU(MENUITEM_About, EditorFrame::About)
	EVT_TEXT(TEXTCTRL_Main, EditorFrame::ChangeToUnsaved)
	EVT_CLOSE(EditorFrame::Exit)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(EditorFrame::AboutDialogClass, wxDialog) //event handlers (handler, maybe?) for about dialog
	EVT_HYPERLINK(HYPERLINK_About, EditorFrame::AboutDialogClass::OnAboutLink)
END_EVENT_TABLE()

EditorFrame::EditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size): wxFrame (NULL, -1, title, pos, size) { //initialize components
	TextBox = new wxTextCtrl (this, TEXTCTRL_Main, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
	MenuBar = new wxMenuBar;
	FileMenu = new wxMenu;

	ConfirmExitDialog = new wxMessageDialog (this, _("Do you want to save your unsaved changes?"), _("Unsaved Changes"), wxYES_NO | wxCANCEL | wxICON_EXCLAMATION | wxCENTRE);
	OpenFileDialog = new wxFileDialog (this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	SaveFileDialog = new wxFileDialog (this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);


	ExitMenuItem = new wxMenuItem (FileMenu, MENUITEM_Exit, _("Exit"), wxEmptyString);
	SaveMenuItem = new wxMenuItem (FileMenu, MENUITEM_Save, _("Save"), wxEmptyString);
	SaveAsMenuItem = new wxMenuItem (FileMenu, MENUITEM_SaveAs, _("Save As"), wxEmptyString);
	OpenMenuItem = new wxMenuItem (FileMenu, MENUITEM_Open, _("Open"), wxEmptyString);
	ClearMenuItem = new wxMenuItem (FileMenu, MENUITEM_Clear, _("Clear"), wxEmptyString);
	AboutMenuItem = new wxMenuItem (FileMenu, MENUITEM_About, _("About"), wxEmptyString);

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
	FileMenu->Append(AboutMenuItem);

	MenuBar->Append(FileMenu, _("File"));
	SetMenuBar(MenuBar);
}

EditorFrame::AboutDialogClass::AboutDialogClass(wxWindow* parent, const wxString& title, const wxSize& size): wxDialog (parent, -1, title, wxDefaultPosition, size, wxCAPTION | wxCLOSE_BOX | wxSTAY_ON_TOP) { //initialize about dialog
	AboutDialogSizer = new wxBoxSizer (wxVERTICAL);
	AboutText = new wxStaticText (this, -1, "Notepad++++++++++\n\nBy TheWildDefender\n\nMade with wxWidgets\n\nyou need to have a really good reason to download my editor, because it's so bad", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	AboutText->Wrap(290);
	AboutLink = new wxHyperlinkCtrl (this, HYPERLINK_About, "GitHub", "https://www.github.com/TheWildDefender/Notepad-plus10");

	AboutDialogSizer->Add(AboutText, 0, wxALIGN_CENTER_HORIZONTAL);
	AboutDialogSizer->Add(AboutLink, 0, wxALIGN_CENTER_HORIZONTAL);
	AboutDialogSizer->AddStretchSpacer();
	AboutDialogSizer->Add(this->CreateButtonSizer(wxOK), 0, wxALIGN_CENTER_HORIZONTAL);

	this->SetSizer(AboutDialogSizer);
}

void EditorFrame::Exit(wxCommandEvent& event) { //exit function for exit menu item
	if (ConfirmExit() == true) Destroy();
}

void EditorFrame::Exit(wxCloseEvent& event) { //exit function for window close events
	if (ConfirmExit() == true) Destroy();
}

void EditorFrame::Save(wxCommandEvent& event) {
	if (opened_file_name == wxString ("untitled")) {
		SaveAs(null_event);
		saved = true;
	} else TextBox->SaveFile(opened_file_path);
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

void EditorFrame::About(wxCommandEvent& event) {
	AboutDialog->ShowModal();
}

void EditorFrame::AboutDialogClass::OnAboutLink(wxHyperlinkEvent& event) {
	std::minstd_rand generator (std::chrono::system_clock::now().time_since_epoch().count());
	if (generator() % 2 == 0) wxLaunchDefaultBrowser("https://www.github.com/TheWildDefender/Notepad-plus10");
	else wxLaunchDefaultBrowser("https://www.youtube.com/watch?v=diNsdKmye0g");
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
