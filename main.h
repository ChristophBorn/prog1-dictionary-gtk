// Author: Christoph Born (53034, 22/041/62)
// main module (handles GUI)

// ====== dialogs ======

int confirm_dialog(char* question, char* arg1, char* arg2);
/* shows confirmation dialog (blocking)
   - question is printf-like format string to show
   - arg1 & arg2 can be NULL until used in 'question'
   - returns GTK_RESPONSE_YES, GTK_RESPONSE_NO
     or GTK_RESPONSE_DELETE_EVENT (if X on dialog clicked) */

void error_dialog(char* msg, char* arg1, char* secondary);
/* shows error message dialog (blocking)
   - msg is printf-like format string to show
   - arg1 can be NULL until used in question
   - secondary is text to show below msg, NULL for none */

void about_dialog();
/* shows dialog with info about the application (blocking)
   - callback function for Hilfe > Info */

// ====== file handling ======

bool choose_file(char* title, char* btnTitle, int action);
/* shows file chooser-dialog (blocking) & sets chosen file as global 'filename'
   - title is dialog's window title
   - btnTitle is text to show on accept button, e.g., 'Save' or 'Open'
   - action is either GTK_FILE_CHOOSER_ACTION_OPEN or GTK_FILE_CHOOSER_ACTION_SAVE
   - returns true if user chose a file, false if user aborted dialog */

bool file_check_opened();
/* shows error dialog (blocking) if no file is open (global 'filename' still == NULL)
   - returns true if a file is open, false otherwise */

bool file_check_unsaved();
/* shows confirmation dialog (blocking) if there are unsaved changes & saves if wanted
   - returns false if X on dialog was clicked (abort current action then!), true otherwise  */


void file_open();
/* lets user choose file & opens it (sets global 'filename')
   - loads file contents (into global 'dicts') & displays entries
   - on error, partial content may be loaded & displayed
   - callback function for Datei > Öffnen */

void file_new();
/* lets user choose file, creates & opens it (sets global 'filename')
   - clears global 'dicts' & table (GtkTreeView) (if chosen file is writable)
   - callback function for Datei > Neu */


void file_save();
/* saves current entries (global 'dicts') to opened file (global 'filename')
   - callback function for Datei > Speichern */

void file_saveas();
/* lets user choose file & saves current entries (global 'dicts') to it
   - sets this as the new opened file (global 'filename')
   - callback function for Datei > Speichern unter */

// ====== entry display & search ======

void display_entries(tList* dict, int lang);
/* displays entries of 'dict' in table (GtkTreeView)
   - lang is saved as global 'lastLang' to resume display order, e.g., after search ends */


void sort_de();
/* displays current entries (global 'dicts') in german order in table
   - ends search (if any) beforehand by clearing search bar
   - callback function for column title "deutsch" */

void sort_en();
/* displays current entries (global 'dicts') in english order in table
   - ends search (if any) beforehand by clearing search bar
   - callback function for column title "englisch" */


void search();
/* displays entries with matching words for text in search bar & search language
   - if search bar is empty, displays all entries (order determined by global 'lastLang')
   - callback function for change in search bar
   - callback function for change in search language combo box */

// ====== entry manipulation ======

void entry_add();
/* shows input dialog (blocking), adds & displays entry if not aborted
   - callback function for button 'Hinzufügen' */

void entry_remove();
/* shows confirmation dialog (blocking), removes & hides selected entry if not aborted
   - callback function for button 'Entfernen' */

// ====== initialization & termination ======

void quit_app();
/* - shows confirmation dialog (blocking) & saves unsaved changes if wanted
   - frees dynamically allocated application data
   - quits application
   - callback function for Datei > Beenden
   - callback function for main window's X button */