#include <gtk/gtk.h>

void quit_app() {
    // TODO save file & free lists
    g_print("Shutdown hook called ...\n");
    gtk_main_quit();
}

int main(int argc, char* argv[]) {
    GtkBuilder* builder;
    GtkWidget* appWindow;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "gui.glade", NULL);
    appWindow = GTK_WIDGET(gtk_builder_get_object(builder, "app"));
    gtk_builder_connect_signals(builder, NULL);
    //g_object_unref(GOBJECT(builder));

    // TODO dialog untested!
    GtkWidget* dialog;
    dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Speichern?");
    gtk_widget_set_size_request(dialog, 200, 150);
    gtk_widget_show(dialog);
    
    //gtk_widget_show(appWindow);
    gtk_main();

    return 0;
}
