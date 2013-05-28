/*
 * core.h
 * 
 * Copyright 2013 Jamie Cerretelli <jamiecerretelli@gmail.com>
 *
 */
 
#ifndef CORE_H
#define CORE_H

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include "window.h"

typedef struct WIDGETS _widgets;

typedef struct CERT
{
	RSA *rsa;
	BIO *bio;
	EVP_PKEY *pkey;
	gchar *password;
} _cert;

gint init_core(_widgets *widgets);
gint generate_cert(_widgets *widgets, _cert *cs, gint entropy);
gint generate_odd_number();
gchar *generate_string(gchar *out, gint length);
void clean_up(_cert *cs);
gint encrypt_file(_widgets *widgets);
gint decrypt_file(_widgets *widgets);

#endif
