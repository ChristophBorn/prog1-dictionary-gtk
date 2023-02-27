// Author: Christoph Born (53034, 22/041/62)
// main module

#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "dict.h"

#include "main.h"

// application data
tList *dicts[2];
char* filename;
int lastLang = DICT_DE;
bool dataChanged = false; // since last save

// GUI components
GtkWindow* appWindow;
GtkTreeView* treeView;
GtkListStore* liststoreDict;
GtkEntry* searchEntry;
GtkComboBox* cBoxSearchLang;
GtkWidget* dialogAdd;
GtkEntry* entryAddDe;
GtkEntry* entryAddEn;

int confirm_dialog(char* question, char* arg1, char* arg2) {
    int response;    
    GtkWidget* dialog = gtk_message_dialog_new(
        appWindow,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        question, arg1, arg2
    );

    response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return response;
}
void error_dialog(char* msg, char* arg1, char* secondary) {
    GtkWidget* dialog = gtk_message_dialog_new(
        appWindow,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        msg, arg1
    );

    if(secondary)
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", secondary);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
}
void about_dialog() {
    GtkWidget* dialog = gtk_message_dialog_new(
        appWindow,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_CLOSE,
        "Beleg Programmierung 1 (I120) − Wörterbuch mit gtk+"
    );

    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(dialog),
        "von Christoph Born (Matrikelnummer 53034, Studiengruppe 22/041/62)\n"
        "Version %s, compiliert am %s, %s Uhr", PROGRAMVERSION, __DATE__, __TIME__
    );

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
}

bool choose_file(char* title, char* btnTitle, int action) {
    char* dir;
    GtkFileChooser *chooser;
    GtkFileFilter* filterDict = gtk_file_filter_new();
    GtkFileFilter* filterAny = gtk_file_filter_new();
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        title,
        appWindow,
        action,
        "Abbrechen", GTK_RESPONSE_CANCEL,
        btnTitle, GTK_RESPONSE_ACCEPT,
    NULL);

    gtk_file_filter_set_name(filterDict, "Wörterbuch-Datei (.txt)");
    gtk_file_filter_add_pattern(filterDict, "*.txt");

    gtk_file_filter_set_name(filterAny, "Alle Formate");
    gtk_file_filter_add_pattern(filterAny, "*");

    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_add_filter(chooser, filterDict);
    gtk_file_chooser_add_filter(chooser, filterAny);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, true);
    if(action == GTK_FILE_CHOOSER_ACTION_SAVE)
        gtk_file_chooser_set_current_name(chooser, "dict.txt");

    if(filename) {
        dir = g_path_get_dirname(filename);
        gtk_file_chooser_set_current_folder(chooser, dir);
        g_free(dir);
    }

    int res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT) {
        g_free(filename);
        filename = gtk_file_chooser_get_filename(chooser);
    }

    gtk_widget_destroy(dialog);
    
    return (res == GTK_RESPONSE_ACCEPT);
}

bool file_check_opened() {
    if(!filename) {
        error_dialog(
            "Bitte öffnen oder erstellen Sie zuvor eine Wörterbuch-Datei.", NULL,
            "Nutzen Sie dazu die Menüpunkte Datei > Öffnen oder Datei > Neu."
        );
        return false;
    }

    return true;
}

void file_save() {
    if(!file_check_opened())  return;

    if(!dict_write_file(dicts[DICT_DE], filename)) {
        error_dialog(
            "Schreiben von Datei %s ist fehlgeschlagen.", filename,
            "Bitte stellen Sie sicher, dass Sie die Schreibberechtigung für diese Datei besitzen, und versuchen Sie es anschließend erneut."
        );
        return;
    }
    dataChanged = false;
}

bool file_check_unsaved() {
    int response;
    
    if(!dataChanged)  return true;

    response = confirm_dialog(
        "Möchten Sie Ihre Änderungen zuvor speichern?", NULL, NULL
    );
    if(response != GTK_RESPONSE_YES)  return (response == GTK_RESPONSE_NO);
    
    file_save();
    return true;
}

void file_saveas() {
    if(!file_check_opened())  return;

    if(!choose_file("Wörterbuch in anderer Datei speichern", "Speichern", GTK_FILE_CHOOSER_ACTION_SAVE))  return;

    file_save();
}

void quit_app() {
    if(!file_check_unsaved())  return;

    g_free(filename);
    dict_free(dicts);

    gtk_main_quit();
}

void display_entries(tList* dict, int lang) {
    GtkTreeIter iter;
    tDEntry* tmp;

    lastLang = lang;

    // TODO swap colums?

    gtk_list_store_clear(liststoreDict);

    for(tmp = list_get_first(dict); tmp; tmp = list_get_next(dict)) {
        gtk_list_store_append(liststoreDict, &iter);
        gtk_list_store_set(
            liststoreDict, &iter,
            0, tmp->wordDe,
            1, tmp->wordEn,
        -1);
    }
}
void sort_de() {
    if(!file_check_opened())  return;

    gtk_entry_set_text(searchEntry, "");
    display_entries(dicts[DICT_DE], DICT_DE);
}
void sort_en() {
    if(!file_check_opened())  return;

    gtk_entry_set_text(searchEntry, "");
    display_entries(dicts[DICT_EN], DICT_EN);
}

void search() {
    char* query;
    int lang;
    tList* res;

    if(!file_check_opened())  return;

    query = (char *) gtk_entry_get_text(searchEntry);
    if(strlen(query) == 0) {
        display_entries(dicts[lastLang], lastLang);
        g_free(query);
        return;
    }

    lang = strcmp("deutsch",
                  gtk_combo_box_get_active_id(cBoxSearchLang)
    ) == 0 ? DICT_DE : DICT_EN;

    res = dict_search(dicts, lang, query);
    if(!res) {
        error_dialog("Suche ist fehlgeschlagen.", NULL, NULL);
        return;
    }

    display_entries(res, lang);
}

void entry_add() {
    int response;
    char* wordDe;
    char* wordEn;
    
    if(!file_check_opened())  return;
    
    // Clear fields from previous run
    gtk_entry_set_text(entryAddDe, "");
    gtk_entry_set_text(entryAddEn, "");

    response = gtk_dialog_run(GTK_DIALOG(dialogAdd));
    gtk_widget_hide(dialogAdd);
    if(response != GTK_RESPONSE_OK)  return;

    wordDe = (char *) gtk_entry_get_text(entryAddDe);
    wordEn = (char *) gtk_entry_get_text(entryAddEn);
    
    if(!dict_insert(dicts, wordDe, wordEn)) {
        error_dialog("Hinzufügen ist fehlgeschlagen.", NULL, NULL);
    }else{
        display_entries(dicts[lastLang], lastLang);
        dataChanged = true;
    }
}

void entry_remove() {
    GtkTreeIter iter;
    GtkTreeModel* model;
    gchar* wordDe;
    gchar* wordEn;

    if(!file_check_opened())  return;

    if(!gtk_tree_selection_get_selected(
        gtk_tree_view_get_selection(treeView),
        &model, &iter
    )) {
        error_dialog(
            "Bitte markieren Sie zuvor einen Eintrag, den Sie entfernen möchten.",
            NULL, NULL
        );
        return;
    }

    gtk_tree_model_get(
        model, &iter,
        0, &wordDe,
        1, &wordEn,
    -1);

    if(GTK_RESPONSE_YES != confirm_dialog(
        "Möchten Sie den Eintrag \"%s\", \"%s\" wirklich löschen?", wordDe, wordEn
    ))  return;

    if(dict_remove(dicts, wordDe, wordEn))  {
        dataChanged = true;
        gtk_list_store_remove(liststoreDict, &iter);
    }

    g_free(wordDe);
    g_free(wordEn);
}

void file_open() {
    if(!file_check_unsaved())  return;
    
    if(!choose_file("Wörterbuch-Datei öffnen", "Öffnen", GTK_FILE_CHOOSER_ACTION_OPEN))  return;

    dict_clear(dicts);

    if(!dict_read_file(dicts, filename)) {
        error_dialog(
            "Lesen von Datei %s ist fehlgeschlagen.", filename,
            "Bitte stellen Sie sicher, \n"
            "- dass Sie die Schreibberechtigung für diese Datei besitzen\n"
            "- und es tatsächlich eine Wörterbuch-Datei dieses Programms ist (Format Dateiinhalt)\n"
            "und versuchen Sie es anschließend erneut."
        );
        return;
    }
    
    dataChanged = false;
    sort_de(); // show entries
}

void file_new() {
    FILE* pf;
    
    if(!file_check_unsaved())  return;

    if(!choose_file("Wörterbuch-Datei erstellen", "Erstellen", GTK_FILE_CHOOSER_ACTION_SAVE))  return;

    // write test
    pf = fopen(filename, "wt");
    if(!pf) {
        error_dialog(
            "Anlegen von Datei %s ist fehlgeschlagen.", filename,
            "Bitte stellen Sie sicher, dass Sie die Schreibberechtigung für diese Datei bzw. das Verzeichnis besitzen, und versuchen Sie es anschließend erneut."
        );
        return;
    }
    fclose(pf);

    dict_clear(dicts);
    sort_de(); // show entries
    dataChanged = false;
}

int main(int argc, char* argv[]) {
    GtkBuilder* builder;

    // Load from gui.glade
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gui.glade", NULL);
    gtk_builder_connect_signals(builder, NULL);
    appWindow = GTK_WINDOW(gtk_builder_get_object(builder, "app"));
    treeView = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeView"));
    liststoreDict = GTK_LIST_STORE(gtk_builder_get_object(builder, "liststoreDict"));
    searchEntry = GTK_ENTRY(gtk_builder_get_object(builder, "searchEntry"));
    cBoxSearchLang = GTK_COMBO_BOX(gtk_builder_get_object(builder, "cBoxSearchLang"));
    dialogAdd = GTK_WIDGET(gtk_builder_get_object(builder, "dialogAdd"));
    entryAddDe = GTK_ENTRY(gtk_builder_get_object(builder, "entryAddDe"));
    entryAddEn = GTK_ENTRY(gtk_builder_get_object(builder, "entryAddEn"));
    g_object_unref(G_OBJECT(builder));

    // Prepare GUI
    gtk_entry_set_max_length(entryAddDe, DICT_MAX_WORD_LEN);
    gtk_entry_set_max_length(entryAddEn, DICT_MAX_WORD_LEN);

    // Prepare data structure
    dicts[DICT_DE] = list_create();
    dicts[DICT_EN] = list_create();
    
    
    // Enter GUI main loop
    gtk_main();

    return 0;
}
