class EditorApp: public wxApp {
	public:
		virtual bool OnInit();
};

class EditorFrame: public wxFrame {
	public:
		EditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

		wxTextCtrl *TextBox;
		wxMenuBar *MenuBar;
		wxMessageDialog *ConfirmExitDialog;
		wxFileDialog *SaveFileDialog;
		wxFileDialog *OpenFileDialog;

		wxMenuItem *ExitMenuItem;
		wxMenuItem *SaveMenuItem;
		wxMenuItem *SaveAsMenuItem;
		wxMenuItem *OpenMenuItem;
		wxMenuItem *ClearMenuItem;

		void Exit(wxCommandEvent& event);
		void Exit(wxCloseEvent& event);
		void Save(wxCommandEvent& event);
		void SaveAs(wxCommandEvent& event);
		void Open(wxCommandEvent& event);
		void Clear(wxCommandEvent& event);
		void ChangeToUnsaved(wxCommandEvent& event);
		bool ConfirmExit();

		DECLARE_EVENT_TABLE();
};

enum {
	TEXTCTRL_Main = wxID_HIGHEST + 1,
	MENUITEM_Exit,
	MENUITEM_Save,
	MENUITEM_SaveAs,
	MENUITEM_Open,
	MENUITEM_Clear
};

bool saved = true;
bool close_app;
int dialog_input;
wxString opened_file_name ("untitled");
wxString opened_file_path;
wxCommandEvent null_event (wxEVT_NULL);

DECLARE_APP(EditorApp)
