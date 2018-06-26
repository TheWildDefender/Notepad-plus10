class EditorApp: public wxApp {
	public:
		virtual bool OnInit();
};

class EditorFrame: public wxFrame {
	public:
		EditorFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

		class AboutDialogClass: public wxDialog {
			public:
				AboutDialogClass(const wxString& title, const wxSize& size);
				void OnAboutLink(wxHyperlinkEvent& event);

				DECLARE_EVENT_TABLE()
		};

		wxTextCtrl *TextBox;
		wxMenuBar *MenuBar;
		wxMenu *FileMenu;

		wxMessageDialog *ConfirmExitDialog;
		wxFileDialog *SaveFileDialog;
		wxFileDialog *OpenFileDialog;
		AboutDialogClass *AboutDialog;

		wxBoxSizer *AboutDialogSizer;
		wxStaticText *AboutText;
		wxHyperlinkCtrl *AboutLink;

		wxMenuItem *ExitMenuItem;
		wxMenuItem *SaveMenuItem;
		wxMenuItem *SaveAsMenuItem;
		wxMenuItem *OpenMenuItem;
		wxMenuItem *ClearMenuItem;
		wxMenuItem *AboutMenuItem;

		void Exit(wxCommandEvent& event);
		void Exit(wxCloseEvent& event);
		void Save(wxCommandEvent& event);
		void SaveAs(wxCommandEvent& event);
		void Open(wxCommandEvent& event);
		void Clear(wxCommandEvent& event);
		void ChangeToUnsaved(wxCommandEvent& event);
		void About(wxCommandEvent& event);
		bool ConfirmExit(); //ConfirmExit() says "BOOL" in the field of voids				get it... "boo"... "bool"... *runs away*

		DECLARE_EVENT_TABLE()
};

enum { //ids for components
	TEXTCTRL_Main = wxID_HIGHEST + 1,
	MENUITEM_Exit,
	MENUITEM_Save,
	MENUITEM_SaveAs,
	MENUITEM_Open,
	MENUITEM_Clear,
	MENUITEM_About,
	HYPERLINK_About
};

EditorFrame *EditorWindow;
bool saved = true; //states whether the user has unsaved changes or not
bool close_app; //used in confirming exit; states whether editor should close or not
int dialog_input; //stores what button the user clicked in save file dialog
wxString opened_file_name ("untitled"); //stores name of file that is being edited
wxString opened_file_path; //stores path of file that is being edited
wxCommandEvent null_event (wxEVT_NULL); //when you have to pass an argument but you have no argument

DECLARE_APP(EditorApp)
