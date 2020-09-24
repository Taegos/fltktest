#include "EditorWindow.h"
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <functional>
#include <iostream>

#define FL_CB(fn)\
[](Fl_Widget* widget, void* data) { reinterpret_cast<EditorWindow*>(data)->fn(widget); }, this\

#define FL_TEXT_MODIFY_CB(fn)\
[](int pos, int nInserted, int nDeleted,\
   int nRestyled, const char* deletedText,\
   void* data) { \
	   reinterpret_cast<EditorWindow*>(data)->fn(pos, nInserted, nDeleted, nRestyled, deletedText);\
}, this\

EditorWindow::EditorWindow(int w, int h, const char* title) : Fl_Double_Window{w, h}
{
	setup_menu_bar();
	setup_text_editor();
	setup_find_and_replace();
}

EditorWindow::~EditorWindow()
{
}

/***************************************************************************
* Setup
***************************************************************************/

void EditorWindow::setup_menu_bar()
{
	Fl_Menu_Item items[] = {
		{ "&File", 0, 0, 0, FL_SUBMENU },
		{ "&New File", 0, FL_CB(new_file_cb)},
		{ "&Open File", FL_COMMAND + 'o', FL_CB(open_file_cb)},
		{ "&Insert File", FL_COMMAND + 'i', FL_CB(insert_file_cb), FL_MENU_DIVIDER},
		{ "&Save File", FL_COMMAND + 's', FL_CB(save_file_cb)},
		{ "Save File &As", FL_COMMAND + FL_SHIFT + 's', FL_CB(save_file_as_cb), FL_MENU_DIVIDER},
		{ "New &View", FL_ALT + 'v', FL_CB(view_cb)},
		{ "&Close View", FL_COMMAND + 'w', FL_CB(close_view_cb), FL_MENU_DIVIDER},
		{ "E&xit", FL_COMMAND + 'q', FL_CB(exit_cb)},
		{ 0 },

		{ "&Edit", 0, 0, 0, FL_SUBMENU },
		{ "&Undo", FL_COMMAND + 'z', FL_CB(undo_cb), FL_MENU_DIVIDER },
		{ "Cu&t", FL_COMMAND + 'x', FL_CB(cut_cb)},
		{ "&Copy", FL_COMMAND + 'c', FL_CB(copy_cb)},
		{ "&Paste", FL_COMMAND + 'v', FL_CB(paste_cb) },
		{ "&Delete", 0, FL_CB(delete_cb)},
		{ 0 },

		{ "&Search", 0, 0, 0, FL_SUBMENU },
		{ "&Find...", FL_COMMAND + 'f', FL_CB(find_cb) },
		{ "F&ind Again", FL_COMMAND + 'g', FL_CB(find2_cb) },
		{ "&Replace...", FL_COMMAND + 'r', FL_CB(replace_cb) },
		{ "Re&place Again", FL_COMMAND + 't', FL_CB(replace2_cb) },
		{ 0 },
		{ 0 }
	};

	Fl_Menu_Bar* menu = new Fl_Menu_Bar(0, 0, 400, 30);
	menu->copy(items);
}


void EditorWindow::setup_text_editor()
{
	editor = new Fl_Text_Editor(0, 30, 640, 370);
	textbuf = new Fl_Text_Buffer();

	editor->buffer(textbuf);
	editor->textfont(FL_COURIER);
	textbuf->add_modify_callback(FL_TEXT_MODIFY_CB(text_editor_changed_cb));
	textbuf->call_modify_callbacks();
}


void EditorWindow::setup_find_and_replace() 
{
	replace_dlg = new Fl_Window(300, 105, "Replace");
	replace_find = new Fl_Input(70, 10, 200, 25, "Find:");
	replace_with = new Fl_Input(70, 40, 200, 25, "Replace:");
	replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
	replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
	replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
}

//https://www.fltk.org/doc-1.0/editor.html

void EditorWindow::set_title()
{
}

/***************************************************************************
* Menu bar callbacks
***************************************************************************/

void EditorWindow::new_file_cb(Fl_Widget*) {
	//if (!check_save()) return;
	filename[0] = '\0';
	textbuf->select(0, textbuf->length());
	textbuf->remove_selection();
	changed = 0;
	textbuf->call_modify_callbacks();

}

void EditorWindow::open_file_cb(Fl_Widget*)
{
	char* newfile = fl_file_chooser("Open File?", "*", filename);
//	if (newfile != NULL) load_file(newfile, -1);
}

void EditorWindow::insert_file_cb(Fl_Widget*)
{
}

void EditorWindow::save_file_cb(Fl_Widget*)
{
}

void EditorWindow::save_file_as_cb(Fl_Widget*)
{
}

void EditorWindow::view_cb(Fl_Widget*)
{
}

void EditorWindow::close_view_cb(Fl_Widget*)
{
}

void EditorWindow::exit_cb(Fl_Widget*)
{
//	if (changed)
//		if (!check_save())
//			return;
	hide();
}

void EditorWindow::undo_cb(Fl_Widget*)
{
}

void EditorWindow::cut_cb(Fl_Widget*)
{
	Fl_Text_Editor::kf_cut(0, editor);
}

void EditorWindow::copy_cb(Fl_Widget*)
{
	Fl_Text_Editor::kf_copy(0, editor);
}

void EditorWindow::paste_cb(Fl_Widget*)
{
}

void EditorWindow::delete_cb(Fl_Widget*)
{
	textbuf->remove_selection();
}

void EditorWindow::find_cb(Fl_Widget* w)
{
	const char* val;
	val = fl_input("Search String:", search);
	if (val != NULL) {
		// User entered a string - go find it!
		strcpy_s(search, val);
		find2_cb(w);
	}
}

void EditorWindow::find2_cb(Fl_Widget* w)
{
	if (search[0] == '\0') {
		// Search string is blank; get a new one...
		find_cb(w);
		return;
	}
	int pos = editor->insert_position();
	int found = textbuf->search_forward(pos, search, &pos);
	if (found) {
		// Found a match; select and update the position...
		textbuf->select(pos, pos + strlen(search));
		editor->insert_position(pos + strlen(search));
		editor->show_insert_position();
	}
	else fl_alert("No occurrences of \'%s\' found!", search);
}

void EditorWindow::replace_cb(Fl_Widget*)
{
}

void EditorWindow::replace2_cb(Fl_Widget*)
{
}

/***************************************************************************
* Text editor callbacks
***************************************************************************/

void EditorWindow::text_editor_changed_cb(int pos, int nInserted, int nDeleted, int, const char*) {
	if ((nInserted || nDeleted) && !loading) changed = 1;
	set_title();
	if (loading) editor->show_insert_position();
}