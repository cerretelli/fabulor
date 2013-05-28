/*
 * main.c
 * 
 * Copyright 2013 Jamie Cerretelli <jamiecerretelli@gmail.com>
 *
 */
 
#include "window.h"

gint main(gint argc, gchar **argv)
{
	if(window_init() != 0)
	{
		return 1;
	}
	
	return 0;
}
