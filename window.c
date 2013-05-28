/*
 * window.c
 * 
 * Copyright 2013 Jamie Cerretelli <jamiecerretelli@gmail.com>
 *
 */
 
#include "window.h"

gint window_init()
{
    _widgets *widgets;
    widgets = g_malloc(sizeof(_widgets));
    
    gtk_init(NULL, NULL);
    
    if(window_build(widgets) != 0)
    {
        g_printf("[!] window_build failed!\n");
        g_free(widgets);
        return 1;
    }
    
    gtk_widget_show_all(widgets->window);
    gtk_main();
    
    g_free(widgets);
    return 0;
}

//
//
gint window_build(_widgets *widgets)
{
	// window
    widgets->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(widgets->window), "Fabulor");
	gtk_window_set_default_size(GTK_WINDOW(widgets->window), 600, 500);
	gtk_window_set_position(GTK_WINDOW(widgets->window), GTK_WIN_POS_CENTER);
	g_signal_connect(widgets->window, "destroy", G_CALLBACK(on_window_destroy), NULL);
	
	// top level grid
	GtkWidget *top_grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(widgets->window), top_grid);
	gtk_grid_set_column_homogeneous(GTK_GRID(top_grid), 1);
	
	//
	GtkWidget *grid_0 = gtk_grid_new();
	gtk_grid_attach(GTK_GRID(top_grid), grid_0, 0, 0, 1, 1);
	
	// entry for password
	widgets->entry_password = gtk_entry_new();
	widgets->buffer_password = gtk_entry_get_buffer(GTK_ENTRY(widgets->entry_password));
	gtk_entry_set_width_chars(GTK_ENTRY(widgets->entry_password), 30);
	gtk_grid_attach(GTK_GRID(grid_0), widgets->entry_password, 0, 0, 1, 1);
	
	// combo
	widgets->cbo_size = gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(widgets->cbo_size), NULL, "512");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(widgets->cbo_size), NULL, "1024");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(widgets->cbo_size), NULL, "2048");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(widgets->cbo_size), NULL, "4096");
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(widgets->cbo_size), 1);
	
	gtk_grid_attach(GTK_GRID(grid_0), widgets->cbo_size, 1, 0, 1, 1);
	
	// button
	widgets->btn_ok = gtk_button_new_with_label("OK");
	g_signal_connect(widgets->btn_ok, "clicked", G_CALLBACK(btn_ok_clicked), widgets);

	gtk_grid_attach(GTK_GRID(grid_0), widgets->btn_ok, 2, 0, 1, 1);
	
	//
	GtkWidget *grid_1 = gtk_grid_new();
	gtk_grid_attach(GTK_GRID(top_grid), grid_1, 0, 1, 1, 1);
	
	// notebook
	widgets->notebook = gtk_notebook_new();
	gtk_grid_attach(GTK_GRID(grid_1), widgets->notebook, 0, 0, 1, 1);
	
	// log
    widgets->log_window = create_log_window();
    
    GtkWidget *scroll_log_window = gtk_scrolled_window_new(NULL, NULL);
    
    gtk_widget_set_halign(scroll_log_window, GTK_ALIGN_FILL);
    gtk_widget_set_valign(scroll_log_window, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(scroll_log_window, TRUE);
    gtk_widget_set_vexpand(scroll_log_window, TRUE);
    gtk_container_add(GTK_CONTAINER(scroll_log_window), widgets->log_window);
    
    GtkWidget *label_log = gtk_label_new("Log");
    gtk_label_set_width_chars(GTK_LABEL(label_log), 10);
    
    gtk_notebook_append_page(GTK_NOTEBOOK(widgets->notebook), scroll_log_window, label_log);
	
	// public
	widgets->text_public = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets->text_public), 0);
	
	GtkWidget *scroll_public = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_halign(scroll_public, GTK_ALIGN_FILL);
    gtk_widget_set_valign(scroll_public, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(scroll_public, TRUE);
    gtk_widget_set_vexpand(scroll_public, TRUE);
    gtk_container_add(GTK_CONTAINER(scroll_public), widgets->text_public);
	
	// public buffer
	widgets->buffer_public = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->text_public));
	
	GtkWidget *label_public = gtk_label_new("Public");
    gtk_label_set_width_chars(GTK_LABEL(label_public), 10);
    
    gtk_notebook_append_page(GTK_NOTEBOOK(widgets->notebook), scroll_public, label_public);
	
	// private
	widgets->text_private = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets->text_private), 0);
	
	GtkWidget *scroll_private = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_halign(scroll_private, GTK_ALIGN_FILL);
    gtk_widget_set_valign(scroll_private, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(scroll_private, TRUE);
    gtk_widget_set_vexpand(scroll_private, TRUE);
    gtk_container_add(GTK_CONTAINER(scroll_private), widgets->text_private);
    
    // private buffer
	widgets->buffer_private = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->text_private));
	
	GtkWidget *label_private = gtk_label_new("Private");
    gtk_label_set_width_chars(GTK_LABEL(label_private), 10);
    
    gtk_notebook_append_page(GTK_NOTEBOOK(widgets->notebook), scroll_private, label_private);
    
    //
    // file related
    GtkWidget *file_grid = gtk_grid_new();
    GtkWidget *label_file = gtk_label_new("File");
    gtk_label_set_width_chars(GTK_LABEL(label_file), 10);
    
    gtk_notebook_append_page(GTK_NOTEBOOK(widgets->notebook), file_grid, label_file);
    
    // open file
    widgets->entry_open = gtk_entry_new();
	widgets->buffer_open = gtk_entry_get_buffer(GTK_ENTRY(widgets->entry_open));
	gtk_entry_set_width_chars(GTK_ENTRY(widgets->entry_open), 40);
	gtk_grid_attach(GTK_GRID(file_grid), widgets->entry_open, 0, 0, 1, 1);
	
	widgets->btn_open = gtk_button_new_with_label("Open");
	g_signal_connect(widgets->btn_open, "clicked", G_CALLBACK(btn_open_clicked), widgets);
	gtk_grid_attach(GTK_GRID(file_grid), widgets->btn_open, 1, 0, 1, 1);
	
	// save file
	widgets->entry_save = gtk_entry_new();
	widgets->buffer_save = gtk_entry_get_buffer(GTK_ENTRY(widgets->entry_save));
	gtk_grid_attach(GTK_GRID(file_grid), widgets->entry_save, 0, 1, 1, 1);
	
	widgets->btn_save = gtk_button_new_with_label("Save");
	g_signal_connect(widgets->btn_save, "clicked", G_CALLBACK(btn_save_clicked), widgets);
	gtk_grid_attach(GTK_GRID(file_grid), widgets->btn_save, 1, 1, 1, 1);
	
	// encrypt
	widgets->btn_encrypt = gtk_button_new_with_label("Encrypt");
	g_signal_connect(widgets->btn_encrypt, "clicked", G_CALLBACK(btn_encrypt_clicked), widgets);
	gtk_grid_attach(GTK_GRID(file_grid), widgets->btn_encrypt, 0, 2, 1, 1);
    
    // decrypt
	widgets->btn_decrypt = gtk_button_new_with_label("Decrypt");
	g_signal_connect(widgets->btn_decrypt, "clicked", G_CALLBACK(btn_decrypt_clicked), widgets);
	gtk_grid_attach(GTK_GRID(file_grid), widgets->btn_decrypt, 1, 2, 1, 1);
    
    return 0;
}

//
GtkWidget *create_log_window(void)
{
	GtkTreeViewColumn *column;
	GtkCellRenderer *render;
	GtkWidget *tree;
	GtkListStore *store;
	
	tree = gtk_tree_view_new();
	render = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Log", render, "text", LOG_MSG, NULL);
	
	gtk_tree_view_column_set_expand(column, 1);
	gtk_tree_view_insert_column(GTK_TREE_VIEW(tree), column, -1);
	
	store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree), GTK_TREE_MODEL(store));
	
	g_object_unref(store);
	
	return tree;
}

//
//
void on_window_destroy(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

//
void btn_ok_clicked(GtkWidget *widget, gpointer data)
{
	_widgets *widgets = data;
	
	if(init_core(widgets) != 0)
	{
		// TODO:
	}
}

void btn_encrypt_clicked(GtkWidget *widget, gpointer data)
{
	_widgets *widgets = data;
	
	if(encrypt_file(widgets) != 0)
	{
	}
}

void btn_decrypt_clicked(GtkWidget *widget, gpointer data)
{
}

// open file
void btn_open_clicked(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
	_widgets *widgets = data;

	dialog = gtk_file_chooser_dialog_new("Open File",
                                      GTK_WINDOW(widgets->window),
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                      NULL);

	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename;

		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
		gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(widgets->buffer_open), filename, strlen(filename));
		
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}

// save file
void btn_save_clicked(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
	_widgets *widgets = data;

	dialog = gtk_file_chooser_dialog_new("Save File",
                                      GTK_WINDOW(widgets->window),
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                      NULL);
                                      
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename;

		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(widgets->buffer_save), filename, strlen(filename));
		g_free (filename);
	}

	gtk_widget_destroy(dialog);
}

// push message to window
void log_message(_widgets *widgets, gchar *message)
{
	GtkListStore *store;
	GtkTreeIter iter;
	
	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(widgets->log_window)));
	
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, LOG_MSG, message, -1);
}
