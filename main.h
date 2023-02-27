// Author: Christoph Born (53034, 22/041/62)
// main module

int confirm_dialog(char* question, char* arg1, char* arg2);
/* shows confirmation dialog (blocking)
   - question is printf-like format string to show
   - arg1 & arg2 can be NULL until used in question
   - returns GTK_RESPONSE_YES, GTK_RESPONSE_NO or GTK_RESPONSE_DELETE_EVENT */
void error_dialog(char* msg, char* arg1, char* secondary);
/* shows error message dialog (blocking)
   - msg is printf-like format string to show
   - arg1 can be NULL until used in question
   - secondary is text to show below msg */
void about_dialog();
/* shows message dialog about the application (blocking)
   - callback fucntion for Hilfe > Info */

bool choose_file(char* title, char* btnTitle, int action);
/* shows filechooser dialog (blocking) & sets chosen file as 'filename'
   - title is dialog's window title
   - btnTitle is text to show on accept button
   - action is a Gtk.FileChooserAction for the dialog
   - returns true if user chose a file, false if user aborted dialog */

bool file_check_opened();
/* shows error dialog (blocking) if no file is open ('filename' == NULL)
   - returns true if a file is open, false otherwise */

void file_save();
/* saves 'dicts' to file with 'filename'
   - callback function for Datei > Speichern */

bool file_check_unsaved();
/* shows confirmation dialog if there are unsaved changes & saves if wanted
   - returns false if X on dialog was clicked (abort current action then!), true otherwise  */

void file_saveas();
/* lets user chose file & saves 'dicts' to thisfile with new 'filename'
   - callback function for Datei > Speichern unter */

void quit_app();

void display_entries(tList* dict, int lang);
void sort_de();
void sort_en();

void search();

void entry_add();

void entry_remove();

void file_open();

void file_new();