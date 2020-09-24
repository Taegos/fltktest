#pragma once
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Return_Button.H"
#include "FL/Fl_Text_Editor.H"
#include "FL/fl_ask.H"

class EditorWindow : public Fl_Double_Window {
public:
	EditorWindow(int, int, const char*);
	~EditorWindow();

private:

	Fl_Window* replace_dlg;
	Fl_Input* replace_find;
	Fl_Input* replace_with;
	Fl_Button* replace_all;
	Fl_Return_Button* replace_next;
	Fl_Button* replace_cancel;

	Fl_Text_Editor* editor;
	char search[256];

	int changed = 0;
	char filename[256] = "asd";
	Fl_Text_Buffer* textbuf;
	int loading = 0;

	void setup_menu_bar();
	void setup_text_editor();
	void setup_find_and_replace();

	void set_title();

	//File
	void new_file_cb(Fl_Widget*);
	void open_file_cb(Fl_Widget*);
	void insert_file_cb(Fl_Widget*);
	void save_file_cb(Fl_Widget*);
	void save_file_as_cb(Fl_Widget*);
	void view_cb(Fl_Widget*);
	void close_view_cb(Fl_Widget*);
	void exit_cb(Fl_Widget*);

	//Edit
	void undo_cb(Fl_Widget*);
	void cut_cb(Fl_Widget*);
	void copy_cb(Fl_Widget*);
	void paste_cb(Fl_Widget*);
	void delete_cb(Fl_Widget*);

    //Search
	void find_cb(Fl_Widget*);
	void find2_cb(Fl_Widget*);
	void replace_cb(Fl_Widget*);
	void replace2_cb(Fl_Widget*);

	//Text editor
	void text_editor_changed_cb(int, int, int, int, const char*);
};