/*
 * window.h
 * 
 * Copyright 2013 Jamie Cerretelli <jamiecerretelli@gmail.com>
 *
 */
 
#ifndef WINDOW_H
#define WINDOW_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <string.h>

#include "core.h"

typedef struct WIDGETS
{
	GtkWidget *window;
	GtkWidget *entry_password;
	GtkWidget *cbo_size;
	GtkWidget *btn_ok;
	GtkWidget *notebook;
	GtkWidget *text_public;
	GtkWidget *text_private;
	GtkWidget *log_window;
	//
	GtkWidget *entry_open;
	GtkWidget *entry_save;
	GtkWidget *btn_open;
	GtkWidget *btn_save;
	GtkWidget *btn_encrypt;
	GtkWidget *btn_decrypt;
	GtkEntryBuffer *buffer_open;
	GtkEntryBuffer *buffer_save;
	
	GtkEntryBuffer *buffer_password;
	GtkTextBuffer *buffer_public;
	GtkTextBuffer *buffer_private;

} _widgets;

enum
{
	LOG_MSG = 0,
	NUM_COLS
};

gint window_init();
gint window_build(_widgets *widgets);
GtkWidget *create_log_window(void);
void log_message(_widgets *widgets, gchar *message);

// callbacks
void on_window_destroy(GtkWidget *widget, gpointer data);
void btn_ok_clicked(GtkWidget *widget, gpointer data);
void btn_open_clicked(GtkWidget *widget, gpointer data);
void btn_save_clicked(GtkWidget *widget, gpointer data);
void btn_encrypt_clicked(GtkWidget *widget, gpointer data);
void btn_decrypt_clicked(GtkWidget *widget, gpointer data);

#endif
